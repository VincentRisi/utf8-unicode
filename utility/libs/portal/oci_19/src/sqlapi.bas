Attribute VB_Name = "SQLAPI_V5"
'*** BB&D Common Routine : Copyright Barone, Budge And Dominick ***'
Public ErrMsg  As String * 1025
Public Const ERR_MSG_LEN = 1024

Declare Function DBVBLogon Lib "sqlapi32.dll" (Magic As Long, ByVal FileName As String, ByVal User As String) As Integer
Declare Function DBLogoff Lib "sqlapi32.dll" (ByVal Magic As Long) As Integer

'Declare Function DBOpen Lib "sqlapi32.dll" (ByVal Magic As Long, Cursor As Long, ByVal Query As String, aData As Any) As Integer
Declare Function DBClose Lib "sqlapi32.dll" (ByVal Cursor As Long) As Integer

Declare Function DBCommit Lib "sqlapi32.dll" (ByVal Magic As Long) As Integer
Declare Function DBRollback Lib "sqlapi32.dll" (ByVal Magic As Long) As Integer

Declare Function DBXTAlloc Lib "sqlapi32.dll" (ByVal How As Integer) As Integer

Declare Function DBRunProc Lib "sqlapi32.dll" (ByVal Magic As Long, ByVal Query As String, aData As Any) As Integer

Declare Function DBOpenExec Lib "sqlapi32.dll" (ByVal Magic As Long, Cursor As Long, ByVal Query As String, aData As Any) As Integer
'Declare Function DBExec Lib "sqlapi32.dll" (ByVal Cursor As Long) As Integer
'Declare Function dbfetchdata Lib "sqlapi32.dll" (ByVal Cursor As Long) As Integer
Declare Function DBFetchData Lib "sqlapi32.dll" (ByVal Cursor As Long, aData As Any) As Integer
Declare Function DBFetchMany Lib "sqlapi32.dll" (ByVal Cursor As Long, No As Integer, aData As Any) As Integer
Declare Function DBRowsFetched Lib "sqlapi32.dll" (ByVal Cursor As Long) As Integer

Declare Function DBCancel Lib "sqlapi32.dll" (ByVal Cursor As Long) As Integer

Declare Function DBSetNull Lib "sqlapi32.dll" (ByVal Cursor As Long, ByVal FieldName As String, ByVal How As Integer) As Integer
Declare Function DBTestNull Lib "sqlapi32.dll" (ByVal Cursor As Long, ByVal FieldName As String, How As Integer) As Integer

Declare Function DBEOD Lib "sqlapi32.dll" (ByVal Cursor As Long) As Integer
Declare Function DBCBEOD Lib "sqlapi32.dll" (ByVal Magic As Long) As Integer

Declare Function DBVBCBError Lib "sqlapi32.dll" (ByVal Magic As Long, ByVal Message As String, ByVal Length As Integer) As Integer
Declare Function DBVBCursorError Lib "sqlapi32.dll" (ByVal Cursor As Long, ByVal Message As String, ByVal Length As Integer) As Integer
Declare Function DBVBCommand Lib "sqlapi32.dll" (ByVal Cursor As Long, ByVal CommandName As String, ByVal Length As Integer) As Integer

Declare Function DBOneWayCipher Lib "sqlapi32.dll" (ByVal TheInput As String, ByVal InpLen As Integer, ByVal TheOutput As String, ByVal OutLen As Integer, ByVal Seed As Integer) As Integer

Public mc As Long
Public Cursor As Long

Public Const SQLapiOK = 0
Public Const SQLapiMemErr = 8001
Public Const SQLapiBinFileErr = 8002
Public Const SQLapiConnectErr = 8003
Public Const SQLapiNoQueryErr = 8004
Public Const SQLapiOpenErr = 8005
Public Const SQLapiParseErr = 8006
Public Const SQLapiBindErr = 8007
Public Const SQLapiDefineErr = 8008
Public Const SQLapiExecErr = 8009
Public Const SQLapiFetchErr = 8010
Public Const SQLapiCancelErr = 8011
Public Const SQLapiClose = 8012
Public Const SQLapiCommitErr = 8013
Public Const SQLapiRollBackErr = 8014
Public Const SQLapiNoMoreRows = 8015
Public Const SQLapiCBErr = 8016
Public Const SQLapiCursorErr = 8017
Public Const SQLapiLastErr = 8018

Sub DBVBErrorCheck(aSession, Erc As Integer, Msg As String)
  If Erc <> SQLapiOK Then
    Erc = DBVBCBError(aSession, ErrMsg, 1024)
    Beep
    MsgBox ErrMsg, vbOKOnly + vbCritical, Msg
    Screen.ActiveForm.Refresh
  End If
End Sub

