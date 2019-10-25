// $Id: GraphFileManager.cpp,v 1.3 2011/04/05 15:04:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "GraphFileManager.h"
#include "Util.h"
#include <algorithm>

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
	m_graphVec.clear();
    m_GraphFileObjects.RemoveAll();
    TRACE("GraphFileManager:Reset\n");
}

void GraphFileManager::SaveFileName( CString &fileName, GraphType gt, BOOL includeToResultTable )
{
	GraphFileObject object;
	object.fileName = fileName;
	object.type = gt;
	object.includeToResultTable = includeToResultTable;
	m_GraphFileObjects.AddHead( object );

	m_graphVec.push_back(object);

    TRACE("GraphFileManager::SaveFileName, %s, size:%d, inc:%d\n",fileName, m_GraphFileObjects.GetSize(), includeToResultTable);
}

int GraphFileManager::GetNoOfSavedFiles( void )
{
	return( (int)m_GraphFileObjects.GetCount() );
}

CString GraphFileManager::GetFileName( int index )
{
	if (index >= m_graphVec.size())
		return "";

	return m_graphVec[index].fileName;	
}

BOOL GraphFileManager::GetIncludeToResultTable( int index )
{
	if (index >= m_graphVec.size())
		return false;

	return m_graphVec[index].includeToResultTable;
}

BOOL GraphFileManager::GetIncludeToResultTable( CString &fileName )
{
	
	return FALSE;
}

void GraphFileManager::IncludeToResultTable( BOOL include, CString &fileName )
{

}

void GraphFileManager::ClearAllIncludeToResultTable( void )
{
}

void GraphFileManager::Sort()
{
	sort(m_graphVec.begin(), m_graphVec.end(), [](const GraphFileObject& lhs, const GraphFileObject& rhs)	{ return lhs.type < rhs.type;	});
}

