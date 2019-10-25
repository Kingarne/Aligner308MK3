// $Id: GraphFileManager.h,v 1.2 2009/10/30 15:06:45 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

enum GraphType
{
	GT_Live=0,
	GT_Polar,
	GT_Sin, 
	GT_Error
};

struct GraphFileObject
{
    GraphFileObject():fileName(""),includeToResultTable(FALSE){}
    CString fileName;
	GraphType type;
	BOOL includeToResultTable;    
};//GraphFileObject;

class CResultTable;

class GraphFileManager
{
public:
	GraphFileManager( void );
	virtual ~GraphFileManager( void );

    void Reset( void );
	void SaveFileName( CString &fileName, GraphType gt, BOOL includeToResultTable );
	void IncludeToResultTable( BOOL include, CString &fileName );
	void ClearAllIncludeToResultTable( void );
	int GetNoOfSavedFiles( void );
	CString GetFileName( int index );
	BOOL GetIncludeToResultTable( int index );
	BOOL GetIncludeToResultTable( CString &fileName );    
	void Sort();

private:

	vector< GraphFileObject> m_graphVec;
	CList<GraphFileObject,GraphFileObject&> m_GraphFileObjects;
};
