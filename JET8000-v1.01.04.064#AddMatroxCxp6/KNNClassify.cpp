// KNNClassify.cpp: implementation of the CKNNClassify class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "KNNClassify.h"
//-------------------------------------------------------------------------------------//
#include "SortObj.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CKNNClassify::CKNNClassify()
{

}
//-------------------------------------------------------------------------------------//
CKNNClassify::~CKNNClassify()
{

}
//-------------------------------------------------------------------------------------//
bool CKNNClassify::CalcCosineSimilarity(const std::vector<double> &L1, const std::vector<double> &L2, double &Similarity)
{
	Similarity = 0.0;
	const size_t L1Cnt = L1.size();
	const size_t L2Cnt = L2.size();
	if ( L1Cnt != L2Cnt ) { return false; }

	size_t i=0;
	double A=0.0, D1=0.0, D2=0.0;
	for ( i=0; i<L1Cnt; i++ )
	{
		A += L1[i]*L2[i];
		D1 += L1[i]*L1[i];
		D2 += L2[i]*L2[i];
	}
	Similarity = A/(sqrt(D1)*sqrt(D2));
	return true;
}
//-------------------------------------------------------------------------------------//
bool CKNNClassify::ExecKNNClassify(const TKnnNode &Node, const std::vector<TKnnNode> &TrainList, int Remainder, int &Label)
{
	size_t     i=0;
	size_t     j=0;	
	size_t     idx=0;
	int        nCnt=0;	
	int        nLab=0;
	double     Similarity=0.0;
	CSortObj   SortObj_Sim;
	CSortObj   SortObj_Label;
	std::set<int> LabelSet;
	std::vector<CSortObj> SortList_Sim;
	std::vector<CSortObj> SortList_Label;
	std::vector<double> SimilarityList;
	const size_t TrainCnt=TrainList.size();

	SortList_Sim.clear();	
	SortObj_Sim.SetSortMode(SORT_BY_DBL);
	for ( i=0; i<TrainCnt; i++ )
	{
		if ( CalcCosineSimilarity(Node.FeatureList, TrainList[i].FeatureList, Similarity) == false )
		{	return false; }

		SortObj_Sim.SetID(i);		
		SortObj_Sim.SetValueDbl(Similarity);
		SortObj_Sim.SetValueInt(TrainList[i].Label);
		SortList_Sim.push_back(SortObj_Sim);

		LabelSet.insert(TrainList[i].Label);
		//SimilarityList.push_back(Similarity);
	}

	i = 0;	
	std::set<int>::iterator iter;
	SortList_Label.clear();
	SortObj_Label.SetSortMode(SORT_BY_CNT);
	for ( iter=LabelSet.begin(); iter!=LabelSet.end(); iter++ )	
	{	
		SortObj_Label.SetID(i);
		SortObj_Label.SetValueCnt(0);
		SortObj_Label.SetValueInt(*iter);
		SortList_Label.push_back(SortObj_Label);
		i ++;		
	}
	const size_t SortCount = SortList_Sim.size();
	const size_t LabelCount=SortList_Label.size();
	if ( 0 == LabelCount )
	{	return false; }

	std::sort(SortList_Sim.begin(), SortList_Sim.end());	
	size_t SortCount2 = MIN(SortCount, Remainder);
	for ( i=0; i<SortCount2; i++ )
	{
		idx = SortList_Sim[SortCount-i-1].GetID();
		nLab = SortList_Sim[SortCount-i-1].GetValueInt();		
		for ( j=0; j<LabelCount; j++ )
		{
			if ( SortList_Label[j].GetValueInt() != nLab ) { continue; }
			nCnt = SortList_Label[j].GetValueCnt();
			SortList_Label[j].SetValueCnt(nCnt+1);
			break;
		}		
	}
	std::sort(SortList_Label.begin(), SortList_Label.end());
	Label = SortList_Label[LabelCount-1].GetValueInt();
	return true;
}
//-------------------------------------------------------------------------------------//