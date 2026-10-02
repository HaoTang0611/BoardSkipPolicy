// KNNClassify.h: interface for the CKNNClassify class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KNNCLASSIFY_H__75C743F2_DDD3_423C_9F53_964CB60E2E8F__INCLUDED_)
#define AFX_KNNCLASSIFY_H__75C743F2_DDD3_423C_9F53_964CB60E2E8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
typedef struct tagKnnNode
{
	CString               Name;
	int                   Label;
	std::vector<double>   FeatureList;
	tagKnnNode()
	{
		Name=_T("");
		Label = 0;		
	}
} TKnnNode, *PKnnNode;
//-------------------------------------------------------------------------------------//
class CKNNClassify  
{
	//-------------------------------------------------------------------------------//
protected:
	//-------------------------------------------------------------------------------//
	bool                       CalcCosineSimilarity(const std::vector<double> &L1, const std::vector<double> &L2, double &Similarity);
	//-------------------------------------------------------------------------------//
public:
	CKNNClassify();
	virtual ~CKNNClassify();
	//-------------------------------------------------------------------------------//
	bool                       ExecKNNClassify(const TKnnNode &Node, const std::vector<TKnnNode> &TrainList, int Remainder, int &Label);
	//-------------------------------------------------------------------------------//
};

#endif // !defined(AFX_KNNCLASSIFY_H__75C743F2_DDD3_423C_9F53_964CB60E2E8F__INCLUDED_)
