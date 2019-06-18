Option Explicit

'Declarations Used to Generate DSN
Private Const ODBC_ADD_DSN = 1 ' Add data source
Private Const ODBC_CONFIG_DSN = 2 ' Configure (edit) data source
Private Const ODBC_REMOVE_DSN = 3 ' Remove data source
Private Const vbAPINull As Long = 0& ' NULL Pointer

Private Declare Function SQLConfigDataSource Lib "ODBCCP32.DLL" _
(ByVal hwndParent As Long, ByVal fRequest As Long, ByVal lpszDriver _
As String, ByVal lpszAttributes As String) As Long

Public Sub CreateDSN(sDSN As String)
    Dim nRet As Long
    Dim sDriver As String
    Dim sAttributes As String

    sDriver = "Microsoft Access Driver (*.mdb)"
    sAttributes = "DSN=" & sDSN & Chr$(0)
    sAttributes = sAttributes & "DBQ=C:\Program Files (x86)\Schill Reglerteknik AB\Aligner 308 MK2\Aligner308.mdb" & Chr$(0)

	WS.
	 'DBEngine.RegisterDatabase "pubs", "SQL Server", True, sAttributes

    nRet = SQLConfigDataSource(vbAPINull, 1, sDriver, sAttributes)
End Sub

Public Sub DeleteDSN(sDSN As String)
    Dim nRet As Long
    Dim sDriver As String
    Dim sAttributes As String

    sDriver = "SQL Server"
    sAttributes = sAttributes & "DSN=" & sDSN & Chr$(0)

    nRet = SQLConfigDataSource(vbAPINull, 2, sDriver, sAttributes)

End Sub

Private Sub Form_Load()
    CreateDSN "Aligner308"
   
End Sub