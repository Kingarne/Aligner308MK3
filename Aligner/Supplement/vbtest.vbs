
If Not IsEmpty(Session) Then
	customdata=Session.Property("CustomActionData")
Else
	customdata = "SomeOtherSetting"
End If

Msgbox customdata