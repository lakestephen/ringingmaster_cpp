	// TwoDimArray.h: interface for the TwoDimArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TWODIMARRAY_H__3B2C19A1_3F9A_11D5_B5CA_009027BB3286__INCLUDED_)
#define AFX_TWODIMARRAY_H__3B2C19A1_3F9A_11D5_B5CA_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<class TYPE>
class TwoDArray : public CObject
{
public:
// Construction
	TwoDArray(int width, int height, TYPE val) :
	_width(0),
	_height(0)
	{
		ASSERT(width > 0);
		if (width < 1) width = 1;
		ASSERT(height > 0);	
		if (height < 1) height = 1;

		for (int i=0;i<width*height;i++)
			_data.Add(val);
		_width = width;
		_height = height;
	}

	~TwoDArray()
	{
		_data.RemoveAll();
	}

	int GetWidth() const
	{
		return _width;
	}

	int GetHeight() const
	{
		return _height;
	}
	
	void AddRow(TYPE val, int pos = -1)
	{
		if (pos == -1) pos = _height;

		ASSERT(pos >= 0);
		ASSERT(pos <= _height);
		if ((pos < 0)||(pos > _height)) 
			return;

		_data.InsertAt((_width * pos), val, _width);

		_height++;		
	}

	void AddColumn(TYPE val, int pos = -1)
	{
		if (pos == -1) pos = _width;

		ASSERT(pos >= 0);
		ASSERT(pos <= _width);
		if ((pos < 0)||( pos > _width))
			return;

		for (int i=_height-1;i>=0;i--)
			_data.InsertAt((i*_width)+pos,val);

		_width++;
	}

	void RemoveRow(int pos)
	{
		ASSERT(pos >= 0);
		ASSERT(pos <= _height-1);
		ASSERT(_data.GetSize() > 0);
		if ((pos < 0)||(pos > _height-1)||(_data.GetSize()<=0)) 
			return;

		_data.RemoveAt((_width * pos), _width);
		_height--;		
	}

	void RemoveColumn(int pos)
	{
		ASSERT(pos >= 0);
		ASSERT(pos <= _width-1);
		ASSERT(_data.GetSize() > 0);
		if ((pos < 0)||(pos > _width-1)||(_data.GetSize()<=0)) 
			return;

		for (int i=_height-1;i>=0;i--)
			_data.RemoveAt((i*_width)+pos);
		_width--;
	}

	TYPE GetAt(int x,int y) const
	{
		ASSERT(x <_width);
		ASSERT(y <_height);

		int pos = ((y*(_width)) + x);

		ASSERT((pos < _data.GetSize())&&(pos >= 0));
		return _data.GetAt(pos); 
	}

	void SetAt(TYPE val, int x,int y)
	{
		int pos = ((y*(_width)) + x);

		ASSERT((pos < _data.GetSize())&&(pos >= 0));
		_data.SetAt(pos, val); 
	}

	bool IsValid(CPoint pos) const
	{
		return ((pos.x >= 0)     &&
				(pos.x <  _width)&&
				(pos.y >= 0)     &&
				(pos.y <  _height));
	}


// Implementation
protected:
	CArray<TYPE, TYPE> _data;	
	int _width;
	int _height;

};



#endif // !defined(AFX_TWODIMARRAY_H__3B2C19A1_3F9A_11D5_B5CA_009027BB3286__INCLUDED_)
			 