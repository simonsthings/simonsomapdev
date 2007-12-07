/** ============================================================================
 *  @file   list.h
 *
 *  @path   $(DSPLINK)\gpp\src\gen
 *
 *  @desc   Declarations of list management control structures and definitions
 *          of inline list management functions.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (LIST_H)
#define LIST_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @macro  LIST_IsEmpty
 *
 *  @desc   Macro to check for an empty list.
 *  ============================================================================
 */
#define LIST_IsEmpty(l) (((l)->head.next == &(l)->head))


/** ============================================================================
 *  @name   ListElement
 *
 *  @desc   An element of a list.
 *
 *  @field  next
 *              Next node pointer.
 *  @field  prev
 *              Previous node pointer.
 *  ============================================================================
 */
typedef struct ListElement_tag {
    struct ListElement_tag *  next ;
    struct ListElement_tag *  prev ;
} ListElement ;

/** ============================================================================
 *  @name   List
 *
 *  @desc   Definition of a List.
 *
 *  @field  head
 *              Head of the list.
 *  ============================================================================
 */
typedef struct List_tag {
    ListElement    head ;
} List ;


/** ============================================================================
 *  @func   LIST_Initialize
 *
 *  @desc   Initializes private state of LIST sub-component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *
 *  @enter  None.
 *
 *  @leave  LIST is initialized.
 *
 *  @see    LIST_Finalize
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LIST_Initialize () ;


/** ============================================================================
 *  @func   LIST_Finalize
 *
 *  @desc   Discontinues usage of module.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *
 *  @enter  LIST must be Initialized.
 *
 *  @leave  Resources used by module are freed when reference count reaches
 *          zero.
 *
 *  @see    LIST_Initialize
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LIST_Finalize () ;


/** ============================================================================
 *  @func   LIST_Create
 *
 *  @desc   Allocates and initializes a circular list.
 *          Uses portable MEM_Calloc () function to allocate a list containing
 *          a single element and initializes that element to indicate that it
 *          is the "end of the list" (i.e., the list is empty).
 *          An empty list is indicated by the "next" pointer in the element
 *          at the head of the list pointing to the head of the list, itself.
 *          The created list contains a single element.  This element is the
 *          "empty" element, because its "next" and "prev" pointers point at
 *          the same location (the element itself).
 *
 *  @arg    list
 *              OUT parameter containing the created list.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EOUTOFMEMORY
 *              Out of memory.
 *
 *  @enter  LIST must be initialized.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LIST_Create (OUT List ** list) ;


/** ============================================================================
 *  @func   LIST_Delete
 *
 *  @desc   Removes a list by freeing its control structure's memory space.
 *          Uses portable MEM_Free () function to deallocate the memory
 *          block pointed at by the input parameter.
 *          Must ONLY be used for empty lists, because it does not walk the
 *          chain of list elements.  Calling this function on a non-empty list
 *          will cause a memory leak.
 *
 *  @arg    list
 *              Pointer to list control structure of list to be deleted.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *
 *  @enter  LIST must be initialized.
 *          List must be valid.
 *
 *  @leave  None
 *
 *  @see    LIST_Create
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LIST_Delete (IN List * list) ;


/** ============================================================================
 *  @func   LIST_InitializeElement
 *
 *  @desc   Initializes a list element to default (cleared) values.
 *          This function must not be called to "reset" an element in the middle
 *          of a list chain -- that would break the chain.
 *
 *  @arg    element
 *              Pointer to list element to be reset.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *
 *  @enter  LIST must be initialized.
 *
 *  @leave  None
 *
 *  @see    LIST_InsertBefore, LIST_PutTail
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LIST_InitializeElement (IN ListElement * element) ;


/** ============================================================================
 *  @func   LIST_InsertBefore
 *
 *  @desc   Inserts the element before the existing element.
 *
 *  @arg    list
 *              Pointer to list control structure.
 *  @arg    insertElement
 *              Pointer to element in list to insert.
 *  @arg    existingElement
 *              Pointer to existing list element.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *
 *  @enter  LIST must be initialized.
 *          list must be valid.
 *          pInsertElement must be valid.
 *          pExistingElement must be valid.
 *
 *  @leave  None
 *
 *  @see    LIST_Create, LIST_PutTail
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LIST_InsertBefore (IN  List *          list,
                   IN  ListElement *   insertElement,
                   IN  ListElement *   existingElement) ;


/** ============================================================================
 *  @func   LIST_PutTail
 *
 *  @desc   Adds the specified element to the tail of the list.
 *          Sets new element's "prev" pointer to the address previously held by
 *          the head element's prev pointer.  This is the previous tail member.
 *          of the list.
 *          Sets the new head's prev pointer to the address of the element.
 *          Sets next pointer of the previous tail member of the list to point
 *          to the new element (rather than the head, which it had been
 *          pointing at).
 *          Sets new element's next pointer to the address of the head element.
 *          Sets head's prev pointer to the address of the new element.
 *          Because the tail is always "just before" the head of the list (the
 *          tail's "next" pointer points at the head of the list, and the head's
 *          "prev" pointer points at the tail of the list), the list is
 *          circular.
 *
 *  @arg    list
 *              Pointer to list control structure to which *pElem will be
 *              added.
 *  @arg    element
 *              Pointer to list element to be added.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *
 *  @enter  *element and *list must both exist.
 *          LIST must be initialized.
 *
 *  @leave  None
 *
 *  @see    LIST_InsertBefore
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LIST_PutTail (IN List * list, IN ListElement * element) ;


/** ============================================================================
 *  @func   LIST_RemoveElement
 *
 *  @desc   Removes (unlinks) the given element from the list, if the list is
 *          not empty.  Does not free the list element.
 *
 *  @arg    list
 *              Pointer to list control structure.
 *  @arg    element
 *              Pointer to element in list to remove.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *
 *  @enter  LIST must be initialized.
 *          list must be valid.
 *          element must be valid.
 *
 *  @leave  None
 *
 *  @see    LIST_InsertBefore, LIST_Create, LIST_PutTail
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LIST_RemoveElement (IN List * list, IN ListElement * element) ;



/** ============================================================================
 *  @func   LIST_First
 *
 *  @desc   Returns a pointer to the first element of the list, or NULL if
 *          the list is empty.
 *
 *  @arg    list
 *              Pointer to list control structure.
 *  @arg    element
 *              OUT parameter for holding the first element.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *
 *  @enter  LIST must be initialized.
 *          list must be valid.
 *
 *  @leave  None
 *
 *  @see    LIST_Create, LIST_PutTail, LIST_InsertBefore
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LIST_First (IN List * list, OUT ListElement ** element) ;



/** ============================================================================
 *  @func   LIST_GetHead
 *
 *  @desc   Pops the head off the list and returns a pointer to it.
 *          If the list is empty, returns NULL.
 *          Else, removes the element at the head of the list, making the next
 *          element the head of the list.
 *          The head is removed by making the tail element of the list point its
 *          "next" pointer at the next element after the head, and by making the
 *          "prev" pointer of the next element after the head point at the tail
 *          element.  So the next element after the head becomes the new head of
 *          the list.
 *          Because the tail of the list points forward (its "next" pointer) to
 *          the head of the list, and the head of the list points backward (its
 *          "prev" pointer) to the tail of the list, this list is circular.
 *
 *  @arg    list
 *              Pointer to list control structure of list whose head.
 *              element is to be removed.
 *  @arg    pHeadElement
 *              OUT Parameter to hold the head element.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *
 *  @enter  LIST must be initialized.
 *          list must be valid.
 *          pHeadElement must be valid.
 *
 *  @leave  None
 *
 *  @see    LIST_PutTail, LIST_InsertBefore
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LIST_GetHead (IN List * list, OUT ListElement ** pHeadElement) ;


/** ============================================================================
 *  @func   LIST_Next
 *
 *  @desc   Returns a pointer to the next element of the list, or NULL if
 *          the next element is the head of the list or the list is empty.
 *
 *  @arg    list
 *              Pointer to list control structure.
 *  @arg    pCurrentElement
 *              Pointer to element in list to remove.
 *  @arg    pNextElement
 *              OUT parameter to hold the next element.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *
 *  @enter  LIST must be initialized.
 *          list must be valid.
 *          pCurrentElem must be valid.
 *
 *  @leave  None
 *
 *  @see    LIST_InsertBefore, LIST_PutTail
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LIST_Next (IN  List *          list,
           IN  ListElement *   pCurrentElement,
           OUT ListElement **  pNextElement) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (LIST_H) */

