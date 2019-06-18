
'Values for variables on lines 25 - 29, 32, and 36
'must be set prior to running this script.

	On Error Resume Next

	Dim RegObj
	Dim SysEnv

	Set RegObj = WScript.CreateObject("WScript.Shell")

	'***** Specify the DSN parameters *****

    DataSourceName = "Aligner308"
    DatabaseName = "Name_of_DB"
    Description = "Description of connection"
    LastUser = "Default_Username"
    Server = "Put_server_name_here"

	 'if you use SQL Server the driver name would be "SQL Server"
    DriverName = "SQL Server"

	 'Set this to True if Windows Authentication is used
	 'else set to False or comment out
    WindowsAuthentication = True

	'point to DSN in registry
    REG_KEY_PATH = "HKLM\SOFTWARE\ODBC\ODBC.INI\" & DataSourceName

   ' Open the DSN key and check for Server entry
  	lResult = RegObj.RegRead (REG_KEY_PATH & "\Server")

	'if lResult is nothing, DSN does not exist; create it
  	if lResult = "" then

	  'get os version through WSCript Enviroment object
 	  Set SysEnv = RegObj.Environment("SYSTEM")
	  OSVer = UCase(SysEnv("OS"))

 	  'check which os is running so correct driver path can be set
	  Select Case OSVer
	    Case "WINDOWS_NT"
	        DrvrPath = "C:\WinNT\System32"
	    Case Else
	        DrvrPath = "C:\Windows\System"
	  End Select

	  'create entries in registry
	  RegObj.RegWrite REG_KEY_PATH & "\DataBase",DatabaseName,"REG_SZ"
	  RegObj.RegWrite REG_KEY_PATH & "\Description",Description,"REG_SZ"
	  RegObj.RegWrite REG_KEY_PATH & "\LastUser",LastUser,"REG_SZ"
	  RegObj.RegWrite REG_KEY_PATH & "\Server",Server,"REG_SZ"
	  RegObj.RegWrite REG_KEY_PATH & "\Driver",DrvrPath,"REG_SZ"

	  'if WindowsAuthentication set to True,
	  'a trusted connection entry is added to registry
	  'else, SQL Authentication is used.
	  if WindowsAuthentication = True then
	    RegObj.RegWrite REG_KEY_PATH & "\Trusted_Connection","Yes","REG_SZ"
	  end if

	  'point to data sources key
	  REG_KEY_PATH = "HKLM\SOFTWARE\ODBC\ODBC.INI\ODBC Data Sources\" &
	      DataSourceName

	  'and add the name of the new dsn and the driver to use with it
	  RegObj.RegWrite REG_KEY_PATH,DriverName,"REG_SZ"

	  MsgBox DataSourceName & " DSN Created!"

	else
		MsgBox DataSourceName & " DSN already exists!"
	end if

	Set RegObj = Nothing
	Set SysEnv = Nothing
