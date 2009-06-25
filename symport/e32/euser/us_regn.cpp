// Copyright (c) 1994-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of the License "Symbian Foundation License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// e32\euser\us_regn.cpp
// Optimisation note, it may well be worth machine coding the SubRect() function
// this is at the heart of nearly all complex region calculations.
// 
//


#include "us_std.h"

NONSHARABLE_CLASS(TRectKey) : public TKey
	{
public:
	TRectKey(const TRect *aRectList,const TPoint &aOffset);
	virtual TInt Compare(TInt aLeft,TInt aRight) const;
private:
	const TRect *iRectList;
	TBool iDown;
	TBool iRight;
	};

NONSHARABLE_CLASS(TRectSwap) : public TSwap
	{
public:
	inline TRectSwap(TRect *aRectList);
	virtual void Swap(TInt aLeft,TInt aRight) const;
private:
	TRect *iRectList;
	};

inline TRectSwap::TRectSwap(TRect *aRectList)
	{iRectList=aRectList;}

enum {ERegionBufSize=8};

EXPORT_C TRegion::TRegion(TInt aAllocedRects)
//
// Constructor.
//
	: iCount(0),iError(EFalse),iAllocedRects(aAllocedRects)
	{}




EXPORT_C TBool TRegion::IsEmpty() const
/**
Tests whether the region is empty.

@return True, if the region is empty and its error flag is unset;
        false, otherwise.
*/
	{

	return(iCount==0 && !iError);
	}




#ifndef __REGIONS_MACHINE_CODED__
EXPORT_C TRect TRegion::BoundingRect() const
/**
Gets the minimal rectangle that bounds the entire region.

@return The region's minimal bounding rectangle.
*/
	{

	TRect bounds;
    const TRect *pRect;
	const TRect *pEnd;
	if (iCount>0)
		{
		pRect=RectangleList();
		bounds=(*pRect++);
		for (pEnd=pRect+(iCount-1);pRect<pEnd;pRect++)
			{
			if (pRect->iTl.iX<bounds.iTl.iX)
				bounds.iTl.iX=pRect->iTl.iX;
			if (pRect->iTl.iY<bounds.iTl.iY)
				bounds.iTl.iY=pRect->iTl.iY;
			if (pRect->iBr.iX>bounds.iBr.iX)
				bounds.iBr.iX=pRect->iBr.iX;
			if (pRect->iBr.iY>bounds.iBr.iY)
				bounds.iBr.iY=pRect->iBr.iY;
			}
		}
	return(bounds);
	}	
#endif




EXPORT_C const TRect &TRegion::operator[](TInt aIndex) const
/**
Gets a rectangle from the region.

@param aIndex The index of a rectangle within the region's array of rectangles. 
              Indexes are relative to zero.
              
@return The specified rectangle.              
              
@panic USER 81, if aIndex is greater than or equal to the number 
                of rectangles in the region.
*/
	{

	__ASSERT_ALWAYS((TUint)aIndex<(TUint)iCount,Panic(ETRegionOutOfRange));
    return(*(RectangleList()+aIndex));
    }




#ifndef __REGIONS_MACHINE_CODED__
EXPORT_C TBool TRegion::IsContainedBy(const TRect &aRect) const
/**
Tests whether the region is fully enclosed within the specified rectangle.

@param aRect The specified rectangle.
 
@return True, if the region is fully enclosed within the rectangle (their sides 
        may touch); false, otherwise.
*/
	{

    const TRect *pRect1;
    const TRect *pEnd1;
	for (pRect1=RectangleList(),pEnd1=pRect1+iCount;pRect1<pEnd1;pRect1++)
		{
		if (pRect1->iTl.iX<aRect.iTl.iX || pRect1->iBr.iX>aRect.iBr.iX || pRect1->iTl.iY<aRect.iTl.iY || pRect1->iBr.iY>aRect.iBr.iY)
			return(EFalse);
		}
	return(ETrue);
	}




EXPORT_C TBool TRegion::Intersects(const TRect &aRect) const
/**
Tests whether where there is any intersection between this region and the specified rectangle.

@param aRect The specified rectangle.
 
@return True, if there is an intersection; false, otherwise.
*/
	{

    const TRect *pRect1;
    const TRect *pEnd1;
	for (pRect1=RectangleList(),pEnd1=pRect1+iCount;pRect1<pEnd1;pRect1++)
		{
		if (aRect.Intersects(*pRect1))
			return ETrue;
		}
	return EFalse;
	}




EXPORT_C void TRegion::Copy(const TRegion &aRegion)
/**
Copies another region to this region.

The state of the specified region's error flag is also copied.

@param aRegion The region to be copied.
*/
	{

	if (aRegion.iError)
		ForceError();
	else
		{
		if (iError)
			Clear();
		if (SetListSize(aRegion.iCount))
			{
			iCount=aRegion.iCount;
			Mem::Copy(RectangleListW(), aRegion.RectangleList(), sizeof(TRect)*aRegion.iCount);
			}
		}
	}




EXPORT_C void TRegion::Offset(const TPoint &aOffset)
/**
Moves the region by adding a TPoint offset to the co-ordinates of its corners.
	
The size of the region is not changed.
	
@param aOffset The offset by which the region is moved. The region is moved 
               horizontally by aOffset.iX pixels and vertically by aOffset.iY pixels.
*/
 	{

	TRect *pR=RectangleListW();
	const TRect *pE=pR+iCount;
	while (pR<pE)
		{
		pR->Move(aOffset);
		pR++;
		}
	}




EXPORT_C void TRegion::Offset(TInt aXoffset,TInt aYoffset)
/**
Moves the region by adding X and Y offsets to the co-ordinates of its corners.
	
The size of the region is not changed.
	
@param aXoffset The number of pixels by which to move the region horizontally. 
                If negative, the region moves leftwards. 
@param aYoffset The number of pixels by which to move the region vertically. 
                If negative, the region moves upwards.
*/
	{

	Offset(TPoint(aXoffset,aYoffset));
	}




EXPORT_C TBool TRegion::Contains(const TPoint &aPoint) const
/**
Tests whether a point is located within the region.

If the point is located on the top or left hand side of any rectangle in the 
region, it is considered to be within that rectangle and within the region. 

If the point is located on the right hand side or bottom of a rectangle, it 
is considered to be outside that rectangle, and may be outside the region.

@param aPoint The specified point. 

@return True, if the point is within the region; false, otherwise.
*/
	{
	const TRect *pR=RectangleList();
	const TRect *pE=pR+iCount;
	while (pR<pE)
		{
		if (pR->Contains(aPoint))
			return(ETrue);
		pR++;
		}
	return(EFalse);
	}




EXPORT_C void TRegion::SubRect(const TRect &aRect,TRegion *aSubtractedRegion)
/**
Removes a rectangle from this region.

If there is no intersection between the rectangle and this region, then this 
region is unaffected. 

@param aRect             The rectangular area to be removed from this region. 
@param aSubtractedRegion A pointer to a region. If this is supplied, the
                         removed rectangle is added to it. By default this
                         pointer is NULL.
*/
	{
	if (aRect.IsEmpty())
		return;
	TRect *prect=RectangleListW();
	TInt limit=iCount;
	for (TInt index=0;index<limit;)
		{
		if (prect->iBr.iX>aRect.iTl.iX && prect->iBr.iY>aRect.iTl.iY && prect->iTl.iX<aRect.iBr.iX && prect->iTl.iY<aRect.iBr.iY)
			{
			TRect rect(*prect);
			TRect inter(aRect);
			inter.Intersection(*prect);
			DeleteRect(prect);
			if (inter.iBr.iY!=rect.iBr.iY)
				AppendRect(TRect(rect.iTl.iX,inter.iBr.iY,rect.iBr.iX,rect.iBr.iY));
			if (inter.iTl.iY!=rect.iTl.iY)
				AppendRect(TRect(rect.iTl.iX,rect.iTl.iY,rect.iBr.iX,inter.iTl.iY));
			if (inter.iBr.iX!=rect.iBr.iX)
				AppendRect(TRect(inter.iBr.iX,inter.iTl.iY,rect.iBr.iX,inter.iBr.iY));
			if (inter.iTl.iX!=rect.iTl.iX)
				AppendRect(TRect(rect.iTl.iX,inter.iTl.iY,inter.iTl.iX,inter.iBr.iY));
			if (iError)
				break;
			if (aSubtractedRegion!=NULL)
				aSubtractedRegion->AddRect(inter);
			prect=RectangleListW()+index;		// List might have been re-allocated so re-get the pointer
			limit--;
			}
		else
			{
			index++;
			prect++;
			}
		}
	}
#endif




EXPORT_C void TRegion::Union(const TRegion &aRegion)
/**
Replaces this region with the union of it and the specified region.

Note that if the error flag of either this region or the specified region is
set, then this region is cleared and its error flag is set. This frees up
allocated memory.

@param aRegion The region to be joined to this region.
*/
	{
	RRegionBuf<ERegionBufSize> temp;
	temp.Copy(aRegion);
	if (temp.iCount>iCount)
		{
		temp.AppendRegion(*this);
		Copy(temp);
		}
	else
		AppendRegion(temp);
	temp.Close();
	}




#ifndef __REGIONS_MACHINE_CODED__
EXPORT_C void TRegion::Intersection(const TRegion &aRegion1,const TRegion &aRegion2)
/**
Replaces this region with the area of intersection between two specified regions.
	
Notes:
	
1. If the error flag of either of the two specified regions is set, then this 
   region is cleared and its error flag is set. This frees up allocated memory.
	
2. If this region's error flag is already set, then the function has no effect.
	
@param aRegion1 The first region. 
@param aRegion2 The second region.
*/
    {
	if (aRegion1.iError || aRegion2.iError)
		ForceError();
	else
		{
		iCount=0;
		const TRect *pRect1,*pEnd1;
		const TRect *pRect2,*pEnd2;
		for (pRect1=aRegion1.RectangleList(),pEnd1=pRect1+aRegion1.iCount;pRect1<pEnd1;pRect1++)
			{
			for (pRect2=aRegion2.RectangleList(),pEnd2=pRect2+aRegion2.iCount;pRect2<pEnd2;pRect2++)
				{
				if (pRect1->iBr.iX>pRect2->iTl.iX && pRect1->iBr.iY>pRect2->iTl.iY && pRect1->iTl.iX<pRect2->iBr.iX && pRect1->iTl.iY<pRect2->iBr.iY)
					{
					TRect rect(*pRect2);
					rect.Intersection(*pRect1);
					AppendRect(rect);
					}
				}
			}
		}
	}
#endif




EXPORT_C void TRegion::Intersect(const TRegion &aRegion)
/**
Replaces this region with the area of intersection between it and the specified 
region.

Note that if the error flag of either this region or the specified region is
set, then this region is cleared and its error flag is set. This frees up
allocated memory.

@param aRegion The region to be intersected with this region.
*/
	{
	if (iCount!=0)
		{
		RRegionBuf<ERegionBufSize> temp;
		temp.Copy(*this);
		Intersection(temp,aRegion);
		temp.Close();
		}
	}




EXPORT_C void TRegion::AddRect(const TRect &aRect)
/**
Adds a rectangle to this region.

Notes:

1. If this region's error flag is already set, this function has no effect.

2. If the operation causes the capacity of this region to be exceeded, or if 
   memory allocation fails, the region is cleared, freeing up any memory which 
   has been allocated; its error flag is also set.

@param aRect The rectangle to be added to this region.
*/
	{

	if (!aRect.IsEmpty() && !iError)
		{
		if (IsContainedBy(aRect))
			{
			iCount=0;
			AppendRect(aRect);
			}
		else
			{
			RRegionBuf<ERegionBufSize> temp(aRect);
			AppendRegion(temp);
			temp.Close();
			}
		}
	}




EXPORT_C void TRegion::SubRegion(const TRegion &aRegion,TRegion *aSubtractedRegion)
/**
Removes a region.

If there is no area of intersection between the two regions, this region is 
unaffected.

@param aRegion           The region to be removed from this region.
                         If aRegion's error flag is set, then this region is
                         cleared, freeing up any allocated memory, and the
                         error flag is set.
@param aSubtractedRegion If specified, then on return contains the area removed 
                         from this region.
*/
	{
	if (iCount!=0)
		{
		if (aRegion.iError)
			ForceError();
		else
			{
			const TRect *pR=aRegion.RectangleList();
			const TRect *pE=pR+aRegion.iCount;
			while (pR<pE && !iError)
				SubRect(*pR++,aSubtractedRegion);
			if (iError && aSubtractedRegion)
				aSubtractedRegion->ForceError();
			}
		}
	}




#ifndef __REGIONS_MACHINE_CODED__
EXPORT_C void TRegion::ClipRect(const TRect &aRect)
/**
Clips the region to the specified rectangle.

The resulting region is the area of overlap between the region and the rectangle. 
If there is no overlap, all rectangles within this region are deleted and 
the resulting region is empty.

@param aRect The rectangle to which this region is to be clipped.
*/
	{

	for (TInt index=0;index<iCount;)
		{
		TRect *r2=RectangleListW()+index;
		if (r2->iTl.iX<aRect.iTl.iX)
			r2->iTl.iX=aRect.iTl.iX;
		if (r2->iTl.iY<aRect.iTl.iY)
			r2->iTl.iY=aRect.iTl.iY;
		if (r2->iBr.iX>aRect.iBr.iX)
			r2->iBr.iX=aRect.iBr.iX;
		if (r2->iBr.iY>aRect.iBr.iY)
			r2->iBr.iY=aRect.iBr.iY;
		if (r2->IsEmpty())
			DeleteRect(r2);
		else
			index++;
		}
	}
#endif




EXPORT_C void TRegion::Clear()
/**
Clears this region.

This frees up any memory which has been allocated and unsets the error flag.
*/
	{

	if (iAllocedRects>=0)
		{
		User::Free(((RRegion *)this)->iRectangleList);
		((RRegion *)this)->iRectangleList=NULL;
		iAllocedRects=0;
		}
	iCount=0;
	iError=EFalse;
	}





#ifndef __REGIONS_MACHINE_CODED__
EXPORT_C void TRegion::Tidy()
/**
Merges all rectangles within this region which share an adjacent edge of the 
same length.

The function subsequently frees up any allocated but unused memory.
*/
	{
restart:
	if (iCount>1)
		{
		const TRect *pEnd1=RectangleList()+iCount-1;
		const TRect *pEnd2=RectangleList()+iCount;
		TRect *pRect1=RectangleListW();
		for (;pRect1<pEnd1;pRect1++)
			{
			TRect *pRect2=pRect1+1;
			for (;pRect2<pEnd2;pRect2++)
				{
				if (pRect1->iTl.iX==pRect2->iTl.iX && pRect1->iBr.iX==pRect2->iBr.iX)
					{
					if (pRect1->iBr.iY==pRect2->iTl.iY)
						pRect1->iBr.iY=pRect2->iBr.iY;
					else if (pRect1->iTl.iY==pRect2->iBr.iY)
						pRect1->iTl.iY=pRect2->iTl.iY;
					else
						continue;
					}
				else if (pRect1->iTl.iY==pRect2->iTl.iY && pRect1->iBr.iY==pRect2->iBr.iY)
					{
					if (pRect1->iBr.iX==pRect2->iTl.iX)
						pRect1->iBr.iX=pRect2->iBr.iX;
					else if (pRect1->iTl.iX==pRect2->iBr.iX)
						pRect1->iTl.iX=pRect2->iTl.iX;
					else
						continue;
					}
				else
					continue;
				DeleteRect(pRect2);
				goto restart;
				}
			}
		}
	if (iAllocedRects>iCount)
		{
		TRect *newList=(TRect *)User::ReAlloc(((RRegion *)this)->iRectangleList,iCount*sizeof(TRect));
		if (newList!=NULL)
			{
			iAllocedRects=iCount;
			((RRegion *)this)->iRectangleList=newList;
			}
		}
	}
#endif




EXPORT_C TInt TRegion::Sort()
/**
Sorts the region's array of rectangles according to their vertical position 
on the screen.

The sort uses the bottom right hand corner co-ordinates of the rectangles.
The co-ordinates of the top and left hand sides are irrelevant 
to the sort operation.

Higher rectangles take precedence over lower ones. For rectangles at the same 
vertical position, the leftmost takes priority.

Note that the sort order may need to be different from the default if, for example, 
a region is moved downwards so that lower non-overlapping rectangles need 
to be redrawn (and sorted) before higher ones. In this case, use the second 
overload of this function.

@return KErrNone, if the operation is successful; KErrGeneral, otherwise.
*/
	{

	return Sort(TPoint(-1,-1));
	}




EXPORT_C TInt TRegion::Sort(const TPoint &aOffset)
//
// Sort the region for copying to the same display.
//
/**
Sorts the region's array of rectangles according to a specified sort order.

The sort uses the bottom right hand co-ordinates of the rectangles.
The co-ordinates of the top and left hand sides are irrelevant 
to the sort operation

The order of the sort is determined by whether the iX and iY members of aOffset 
are positive, or zero or less. If aOffset.iY is greater than zero,
lower rectangles take precedence over higher rectangles in the list order.
Otherwise, higher rectangles take precedence. For rectangles of equal height,
aOffset.iX becomes relevant to the sort.
If is greater than zero, rightmost rectangles
take precedence. Otherwise, leftmost rectangles take precedence.

Note that the sort order may need to be different from the default if,
for example, a region is moved downwards so that lower non-overlapping
rectangles need to be redrawn (and sorted) before higher ones.

@param aOffset A point whose iX and iY members determine the order of the 
               sort. 

@return KErrNone, if the operation is successful; KErrGeneral, otherwise.
*/
	{
	TRectKey key(RectangleList(),aOffset);
	TRectSwap swap(RectangleListW());
	return(User::QuickSort(iCount,key,swap));
	}




EXPORT_C TRect *TRegion::RectangleListW()
//
// Return a writeable rectangle list.
//
	{
	if (iAllocedRects>=0)
		return(((RRegion *)this)->iRectangleList);
	else if (iAllocedRects&ERRegionBuf)
		return((TRect *)(this+1));
	return((TRect *)(((RRegion *)this)+1));
	}

TBool TRegion::SetListSize(TInt aCount)
	{
	if (iAllocedRects<0)
		{
		if (aCount>(-(iAllocedRects|ERRegionBuf)))
			{
			if ((iAllocedRects&ERRegionBuf) ||
				(((RRegion *)this)->iRectangleList=
				(TRect *)User::Alloc(aCount*sizeof(TRect)))==NULL)
				{
				ForceError();
				return(EFalse);
				}
			iAllocedRects=aCount;
			}
		}
	else if (iAllocedRects<aCount)
		{
		TRect *newList=(TRect *)User::ReAlloc(((RRegion *)this)->iRectangleList,aCount*sizeof(TRect));
		if (newList==NULL)
			{
			ForceError();
			return(EFalse);
			}
		((RRegion *)this)->iRectangleList=newList;
		iAllocedRects=aCount;
		}
	return(ETrue);
	}

#ifndef __REGIONS_MACHINE_CODED__

EXPORT_C void TRegion::ForceError()
/**
Sets the error flag, and clears the region.

This frees up memory allocated to the region.
*/
	{

	Clear();
	iError=ETrue;
	}

void TRegion::AppendRect(const TRect &aRect)
//
// Add a rectangle to the list.
//
	{
	if (!iError)
		{
		TRect *prects=NULL;
		if (iAllocedRects&ERRegionBuf)
			{							// TRegionFix
			if (iCount==-iAllocedRects)
				{						// Can't expand a TRegionFix
				ForceError();
				return;
				}
			prects=(TRect *)(this+1);
			}
		else if (iAllocedRects<0)
			{							// RRegionBuf
			prects=(TRect *)(((RRegion *)this)+1);
			if (iCount==(-(iAllocedRects|ERRegionBuf)))
				{
				iAllocedRects=iCount+((RRegion *)this)->iGranularity;
				if ((((RRegion *)this)->iRectangleList=(TRect *)User::Alloc(iAllocedRects*sizeof(TRect)))==NULL)
					{
					ForceError();
					return;
					}
				Mem::Copy(((RRegion *)this)->iRectangleList, prects, sizeof(TRect)*((RRegion *)this)->iGranularity);
				prects=((RRegion *)this)->iRectangleList;
				}
			}
		else
			{
			RRegion *pr=(RRegion *)this;
			prects=pr->iRectangleList;
			if (iAllocedRects==iCount)
				{
				iAllocedRects+=pr->iGranularity;
				prects=(TRect *)User::ReAlloc(prects, iAllocedRects*sizeof(TRect));
				if (prects==NULL)
					{
					ForceError();
					return;
					}
				pr->iRectangleList=prects;
				}
			}
		*(prects+iCount)=aRect;
		iCount++;
		}
	}

void TRegion::DeleteRect(TRect *aRect)
//
// Delete a specific rectangle in the list.
//
	{

	iCount--;
	Mem::Copy(aRect,aRect+1,((TUint8 *)(RectangleList()+iCount))-((TUint8 *)aRect));
	}
#endif

void TRegion::AppendRegion(TRegion &aRegion)
//
// Append all the rectangles from aRegion to this.
//
	{

	if (aRegion.iError)
		ForceError();
	else
		{
		aRegion.SubRegion(*this);
		const TRect *pR=aRegion.RectangleList();
		const TRect *pE=pR+aRegion.iCount;
		while (pR<pE)
			AppendRect(*pR++);
		}
	}



EXPORT_C RRegion::RRegion()
	: TRegion(0), iGranularity(EDefaultGranularity), iRectangleList(NULL)
/**
Default constructor.

Initialises its granularity to five.
*/
	{}




EXPORT_C RRegion::RRegion(TInt aGran)
	: TRegion(0), iGranularity(aGran), iRectangleList(NULL)
/**
Constructs the object with the specified granularity.

@param aGran The initial value for the region's granularity.
             This value must not be negative.
*/
	{}




EXPORT_C RRegion::RRegion(const TRect &aRect, TInt aGran)
	: TRegion(0), iGranularity(aGran), iRectangleList(NULL)
/**
Constructs the object with the specified rectangle and granularity.

The resulting region consists of the specified single rectangle.

@param aRect The single rectangle with which to initialise the region 
@param aGran The initial value for the region's granularity. By default, 
             this is five.
*/
	{

	if (!aRect.IsEmpty())
		AppendRect(aRect);
	}




EXPORT_C RRegion::RRegion(const RRegion &aRegion)
/**
Copy constructor.

Constructs a new region from an existing one by performing a bit-wise copy.  Both the new and
existing regions are left containing pointers to the same data, so Close() must only be called on
one of them.

Use of this method is not reccomended.

@param aRegion The region to be copied.
*/
	{

	*this=aRegion;
	}




EXPORT_C RRegion::RRegion(TInt aBuf, TInt aGran)
//
// Constructor.
//
	: TRegion(aBuf), iGranularity(aGran), iRectangleList(NULL)
	{}




EXPORT_C RRegion::RRegion(TInt aCount,TRect* aRectangleList,TInt aGran/*=EDefaultGranularity*/)
/**
Constructor that takes ownership of an already created rectangle list.

@param aCount         The number of rectangles in the region.
@param aRectangleList A pointer to the set of rectangles.
@param aGran          The region's granularity.
*/
	: TRegion(aCount), iGranularity(aGran), iRectangleList(aRectangleList)
	{
	iCount=aCount;
	}




EXPORT_C void RRegion::Close()
/**
Closes the region.

Frees up any memory which has been allocated, and unsets the error flag, if 
set. 

The region can be re-used after calling this method.  Its granularity is preserved.
*/
	{

	Clear();
	}




EXPORT_C void RRegion::Destroy()
//
// Destroy
//
/**
Deletes the region.

Frees all memory.

Note this method will delete the RRegion object and therefore it should not be 
invoked on RRegion objects that are not allocated on the heap.  RRegion::Close()
should be used for RRegion objects stored on the stack.

@panic USER 42 if the RRegion object is stored on the stack.
*/
	{

	Clear();
	delete this;
	}




TInt TRectKey::Compare(TInt aLeft,TInt aRight) const
//
// Compares two rectangles for partial ordering.
//
	{

	if (aLeft==aRight)
		return(0);
	const TRect *r1=&iRectList[aLeft];
	const TRect *r2=&iRectList[aRight];
	if (r2->iBr.iY<=r1->iTl.iY)
		return(iDown ? -1 : 1);
	if (r1->iBr.iY<=r2->iTl.iY)
		return(iDown ? 1 : -1);
	if (r2->iBr.iX<=r1->iTl.iX)
		return(iRight ? -1 : 1);
	__ASSERT_DEBUG(r1->iBr.iX<=r2->iTl.iX,Panic(ETRegionInvalidRegionInSort));
	return(iRight ? 1 : -1);
	}

void TRectSwap::Swap(TInt aLeft,TInt aRight) const
//
// Swap two rectangles.
//
	{

	TRect tmp(iRectList[aLeft]);
	iRectList[aLeft]=iRectList[aRight];
	iRectList[aRight]=tmp;
	}

TRectKey::TRectKey(const TRect *aRectList,const TPoint &aOffset)
//
// Rectangle key constructor
//
	{

	iRectList=aRectList;
	if(aOffset.iX>0)
		iRight=ETrue;
	else
		iRight=EFalse;
	if(aOffset.iY>0)
		iDown=ETrue;
	else
		iDown=EFalse;
	}
