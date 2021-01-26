#pragma once


enum VarianceType 
{
	vt_parts,
	vt_even,
	vt_odd
};

class Variance  
{
public:
	void setVarianceType(VarianceType varianceType);
	VarianceType getVarianceType();
	void setGridString(CString gridString);
	bool isInPart(int partIndex) const;
	bool isBenign() const;
	CString getGridString();
	void setOmit(bool omit);
	bool setParts(CString parts);
	CString getDisplayString();
	void Serialize(CArchive& ar);
	void deserialize_v1(CArchive& ar); 
	void deserialize_v2(CArchive& ar); 
	BOOL isOmit();
	CString getVarianceStr();
	Variance();
	Variance(CString gridString);
	Variance(const Variance& variance);
	virtual ~Variance();

	void operator =(const Variance& variance);
	bool operator ==(const Variance &variance) const;

protected:
	void sortParts();
	BOOL _omit;
	Ints _parts;
	VarianceType _varianceType;
};


typedef CArray<Variance*, Variance*> Variances;
