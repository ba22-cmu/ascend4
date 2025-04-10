/* append (string form) element to a list.
 */
void Asc_AppendElementTcl(Asc_DString *dsp, const char *element)
{
    Interp *iPtr = (Interp *) interp;
    Tcl_Obj *elementPtr = Tcl_NewStringObj(element, -1);
    Tcl_Obj *listPtr = Tcl_NewListObj(1, &elementPtr);
    const char *bytes;
    Tcl_Size length;

    if (Tcl_IsShared(iPtr->objResultPtr)) {
	Tcl_SetObjResult(interp, Tcl_DuplicateObj(iPtr->objResultPtr));
    }
    bytes = TclGetStringFromObj(iPtr->objResultPtr, &length);
    if (TclNeedSpace(bytes, bytes + length)) {
	Tcl_AppendToObj(iPtr->objResultPtr, " ", 1);
    }
    Tcl_AppendObjToObj(iPtr->objResultPtr, listPtr);
    Tcl_DecrRefCount(listPtr);
}
