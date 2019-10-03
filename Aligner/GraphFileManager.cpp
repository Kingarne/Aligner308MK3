// $Id: GraphFileManager.cpp,v 1.3 2011/04/05 15:04:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "GraphFileManager.h"
#include "Util.h"

//	GraphFileManager

GraphFileManager::GraphFileManager( void )
{
	Reset();
}

GraphFileManager::~GraphFileManager( void )
{
  // Empty
}

void GraphFileManager::Reset( void )
{
    m_GraphFileObjects.RemoveAll();
    TRACE("GraphFileManager:Reset\n");
}

void GraphFileManager::SaveFileName( CString &fileName, BOOL includeToResultTable )
{
	GraphFileObject object;
	object.fileName = fileName;
	object.includeToResultTable = includeToResultTable;
	m_GraphFileObjects.AddHead( object );

    TRACE("GraphFileManager::SaveFileName, %s, size:%d, inc:%d\n",fileName, m_GraphFileObjects.GetSize(), includeToResultTable);
}

int GraphFileManager::GetNoOfSavedFiles( void )
{
	return( (int)m_GraphFileObjects.GetCount() );
}

CString GraphFileManager::GetFileName( int index )
{
	GraphFileObject object;
	object.fileName = _T("");

	POSITION pos = m_GraphFileObjects.FindIndex( index );

	if( pos != NULL )
	{
		object = m_GraphFileObjects.GetNext( pos );
	}

	return( object.fileName );
}

BOOL GraphFileManager::GetIncludeToResultTable( int index )
{
	GraphFileObject object;
	object.includeToResultTable = FALSE;
	
	POSITION pos = m_GraphFileObjects.FindIndex( index );

	if( pos != NULL )
	{
		object = m_GraphFileObjects.GetNext( pos );
	}
	return( object.includeToResultTable );
}

BOOL GraphFileManager::GetIncludeToResultTable( CString &fileName )
{
	GraphFileObject object;
	POSITION pos;

  for( pos = m_GraphFileObjects.GetHeadPosition(); pos != NULL; )
  {
		object = m_GraphFileObjects.GetNext( pos );

		if( object.fileName == fileName )
		{
			return( object.includeToResultTable );
		}
  }
	return( FALSE );
}

void GraphFileManager::IncludeToResultTable( BOOL include, CString &fileName )
{
	GraphFileObject object;
	POSITION pos;

  for( pos = m_GraphFileObjects.GetHeadPosition(); pos != NULL; )
  {
		object = m_GraphFileObjects.GetNext( pos );

		if( object.fileName == fileName )
		{
			object.includeToResultTable = include;
			if( pos != NULL )
			{
				m_GraphFileObjects.GetPrev( pos );
			}
			else
			{
				pos = m_GraphFileObjects.GetTailPosition();
			}
			m_GraphFileObjects.SetAt( pos, object );
			 TRACE("GraphFileManager::IncludeToResultTable, %s, size:%d, inc:%d\n",fileName, m_GraphFileObjects.GetSize(), include);
            
            return;
		}
  }	
	return;
}

void GraphFileManager::ClearAllIncludeToResultTable( void )
{
	GraphFileObject object;
	POSITION pos;
TRACE("GraphFileManager:ClearAllIncludeToResultTable\n");
  for( pos = m_GraphFileObjects.GetHeadPosition(); pos != NULL; )
  {
		object = m_GraphFileObjects.GetNext( pos );
		object.includeToResultTable = FALSE;
		if( pos != NULL )
		{
			m_GraphFileObjects.GetPrev( pos );
			m_GraphFileObjects.SetAt( pos, object );
			m_GraphFileObjects.GetNext( pos );
		}
		else
		{
			pos = m_GraphFileObjects.GetTailPosition();
			m_GraphFileObjects.SetAt( pos, object );
			pos = NULL;
		}
  }	
	return;
}


/*void GraphFileManager::MoveUnwantedToTemporaryDir()
{
    GraphFileObject object;
    POSITION pos;

    for( pos = m_GraphFileObjects.GetHeadPosition(); pos != NULL; )
    {
        object = m_GraphFileObjects.GetNext( pos );

        if(object.includeToResultTable == FALSE)
        {
            MoveFileToTemporaryDirectory(object.fileName);    
        }
        
    } 
    
}*/

