#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;
class PREDICTOR
{
	public:
	int M_BIMODAL;
	int BTB_SIZE;//common
	int BTB_ASSOC;//common
	string TRACEFILE;//common
	vector<string> TRACES;//common
	int NUMBEROFTRACES;//common
	string TYPE;//common
	vector<int> PREDICTIONTABLE_BIMODAL;
	vector<string> PC;//common
	vector<string> BRANCH_OUTCOME_ACTUAL;//common
	vector<string> PC_BINARY;//common
	vector<string> INDEX_BIMODAL_BINARY;
	vector<float> INDEX_BIMODAL_DECIMAL;
	float PREDICTIONS;//common
	float CORRECTPREDICTIONS;//common
	float MISSPREDICTIONS;//common
	float MISSPREDICTION_RATE;//common
	int M_GSHARE;
	int N_GSHARE;
	vector<int> PREDICTIONTABLE_GSHARE;
	vector<string> INDEX_GSHARE_BINARY;
	vector<float> INDEX_GSHARE_DECIMAL;
	vector<int> GBHR;
	int K_HYBRID;
	vector<string> INDEX_HYBRID_BINARY;
	vector<float> INDEX_HYBRID_DECIMAL;
	vector<string> OUTCOME_BIMODAL;
	vector<string> OUTCOME_GSHARE;
	string FIRSTARG;
	//******************BTB variables***************//
	vector<vector<string> > BTB_TAG;
	vector<vector<int> > BTB_VALIDBITS;
    vector<vector<int> > BTB_LRUCOUNT;
	int BTB_NUMBEROFSETS;
	vector <string> BTB_TAGVALUE;               
    vector <string> BTB_INDEX;         
    string BTB_indicate;
    vector <float> BTB_DECIMALINDEX;      
    //int BRANCH_PREDICTIONS;            
    int BTB_MISSPREDICTIONS;
	PREDICTOR()
	{}
	void decodelogic()
	{
		//Split traces into PC and outcomes.
		for(int i=0;i<NUMBEROFTRACES;i++)
		{
			PC[i]=TRACES[i].substr(0,6);
			BRANCH_OUTCOME_ACTUAL[i]=TRACES[i].substr(7,1);
		}
		//convert PC into binary string
		for(int i=0;i<NUMBEROFTRACES;i++)
		{	
			for(int j=0; j<PC[i].length(); j++)
			{	
				switch (PC[i][j])
				{
					case '0': PC_BINARY[i].append("0000"); 
								break;
					case '1': PC_BINARY[i].append("0001"); 
								break;
					case '2': PC_BINARY[i].append("0010"); 
								break;
					case '3': PC_BINARY[i].append("0011"); 
								break;
					case '4': PC_BINARY[i].append("0100"); 
								break;
					case '5': PC_BINARY[i].append("0101"); 
								break;
					case '6': PC_BINARY[i].append("0110"); 
								break;
					case '7': PC_BINARY[i].append("0111"); 
								break;
					case '8': PC_BINARY[i].append("1000"); 
								break;
					case '9': PC_BINARY[i].append("1001"); 
								break;
					case 'a': PC_BINARY[i].append("1010"); 
								break;
					case 'b': PC_BINARY[i].append("1011"); 
								break;
					case 'c': PC_BINARY[i].append("1100");
								break;
					case 'd': PC_BINARY[i].append("1101"); 
								break;
					case 'e': PC_BINARY[i].append("1110"); 
								break;
					case 'f': PC_BINARY[i].append("1111"); 
								break;
				}
			}
			
		}
		//Tag and index calculation for BTB
		if(BTB_ASSOC!=0)
		{
			int sizeindex = log2(BTB_NUMBEROFSETS);
			int sizetag = 24-sizeindex-2;
			vector<string> tempstore(NUMBEROFTRACES,"");
			for(int i=0; i<NUMBEROFTRACES;i++)
			{
				BTB_TAGVALUE[i]=PC_BINARY[i].substr(0,sizetag);
				tempstore[i]=PC_BINARY[i];
				tempstore[i].erase(0,sizetag);
				BTB_INDEX[i]=tempstore[i].substr(0,sizeindex);
			}
			int indexlength=sizeindex;
			int icount;
			for(int i=0; i<NUMBEROFTRACES; i++)  
			{
				icount=0;
				for(int k=indexlength-1; k>=0; k--)
				{
					if (BTB_INDEX[i][k] == '1')
					{
						BTB_DECIMALINDEX[i]+=pow(2.0,icount);
					}	
					icount++;
				}
			}
		}
		//Index calculation for bimodal
		if(TYPE=="bimodal")
		{
			//binary index for bimodal
			indexbimodal();
			//decimal index for bimodal
			int k;
			int count;
			int indexsize;
			indexsize=INDEX_BIMODAL_BINARY[0].length();
			for(int i=0; i<NUMBEROFTRACES; i++)
			{
				count=0;
				for(k=indexsize-1; k>=0; k--)
				{
					if (INDEX_BIMODAL_BINARY[i][k] == '1')
					{
						INDEX_BIMODAL_DECIMAL[i]+=pow(2.0,count);
					}
					count++;
				}
			}
		}
		//index calculation for g-share
		if(TYPE=="gshare")
		{
			//binary index for gshare
			indexgshare();
			//decimal index for gshare
			int k1;
			int count1;
			int indexsize1;
			indexsize1=INDEX_GSHARE_BINARY[0].length();
			for(int i=0; i<NUMBEROFTRACES; i++)
			{
				count1=0;
				for(k1=indexsize1-1; k1>=0; k1--)
				{
					if (INDEX_GSHARE_BINARY[i][k1] == '1')
					{
						INDEX_GSHARE_DECIMAL[i]+=pow(2.0,count1);
					}
					count1++;
				}
			}
		}
		if(TYPE=="hybrid")
		{
			//binary index for bimodal
			indexbimodal();
			//decimal index for bimodal
			int k;
			int count;
			int indexsize;
			indexsize=INDEX_BIMODAL_BINARY[0].length();
			for(int i=0; i<NUMBEROFTRACES; i++)
			{
				count=0;
				for(k=indexsize-1; k>=0; k--)
				{
					if (INDEX_BIMODAL_BINARY[i][k] == '1')
					{
						INDEX_BIMODAL_DECIMAL[i]+=pow(2.0,count);
					}
					count++;
				}
			}
			//binary index for gshare
			indexgshare();
			//decimal index for gshare
			int k1;
			int count1;
			int indexsize1;
			indexsize1=INDEX_GSHARE_BINARY[0].length();
			for(int i=0; i<NUMBEROFTRACES; i++)
			{
				count1=0;
				for(k1=indexsize1-1; k1>=0; k1--)
				{
					if (INDEX_GSHARE_BINARY[i][k1] == '1')
					{
						INDEX_GSHARE_DECIMAL[i]+=pow(2.0,count1);
					}
					count1++;
				}
			}
			//binary index for hybrid
			int lengthh;
			int x2=K_HYBRID+2;
			string temp2;
			lengthh=PC_BINARY[0].length();
			vector<string> pctempstore2;
			pctempstore2.resize(NUMBEROFTRACES);
			for(int i=0;i<NUMBEROFTRACES;i++)
			{
				pctempstore2[i]=PC_BINARY[i];
				temp2=pctempstore2[i].erase(0,lengthh-x2);
				INDEX_HYBRID_BINARY[i]=temp2.substr(0,K_HYBRID);
			}
			//decimal index for hybrid
			int k2;
			int count2;
			int indexsize2;
			indexsize2=INDEX_HYBRID_BINARY[0].length();
			for(int i=0; i<NUMBEROFTRACES; i++)
			{
				count2=0;
				for(k2=indexsize2-1; k2>=0; k2--)
				{
					if (INDEX_HYBRID_BINARY[i][k2] == '1')
					{
						INDEX_HYBRID_DECIMAL[i]+=pow(2.0,count2);
					}
					count2++;
				}
			}
		}
		
	}
	void indexbimodal()
	{
		int length;
		int x=M_BIMODAL+2;
		string temp;
		length=PC_BINARY[0].length();
		vector<string> pctempstore;
		pctempstore.resize(NUMBEROFTRACES);
		for(int i=0;i<NUMBEROFTRACES;i++)
		{
			pctempstore[i]=PC_BINARY[i];
			temp=pctempstore[i].erase(0,length-x);
			INDEX_BIMODAL_BINARY[i]=temp.substr(0,M_BIMODAL);
		}
	}
	void indexgshare()
	{
		int lengthg;
		int x1=M_GSHARE+2;
		string temp1;
		lengthg=PC_BINARY[0].length();
		vector<string> pctempstore1;
		pctempstore1.resize(NUMBEROFTRACES);
		vector<string> m;
		m.resize(NUMBEROFTRACES);
		vector<string> firstn;
		firstn.resize(NUMBEROFTRACES);
		vector<string> append;
		append.resize(NUMBEROFTRACES);
		vector<string> exorresult;
		exorresult.resize(NUMBEROFTRACES);
		for(int i=0;i<NUMBEROFTRACES;i++)
		{
			pctempstore1[i]=PC_BINARY[i];
			temp1=pctempstore1[i].erase(0,lengthg-x1);
			m[i]=temp1.substr(0,M_GSHARE);
		}
		for(int i=0;i<NUMBEROFTRACES;i++)
		{
			firstn[i]=m[i].substr(0,N_GSHARE);
			append[i]=m[i].erase(0,N_GSHARE);
		}
		lruinit();
		for(int i=0;i<NUMBEROFTRACES;i++)
		{
			for(int j=0;j<N_GSHARE;j++)
			{
				exorresult[i]+=(GBHR[j])^(firstn[i][j]-'0')+'0';
			}
			INDEX_GSHARE_BINARY[i]=exorresult[i].append(append[i]);
			if(BTB_ASSOC!=0)
			{
				
				BTBop(BTB_DECIMALINDEX[i],i);
				if(BTB_indicate=="t")
				{
					for(int k=N_GSHARE-2;k>=0;k--)
					{
						GBHR[k+1]=GBHR[k];
					}
					if(BRANCH_OUTCOME_ACTUAL[i] == "t")
					{
						GBHR[0] = 1;
					}
					else
					{
						GBHR[0] = 0;
					}
				}
			}
			else
			{
				for(int k=N_GSHARE-2;k>=0;k--)
				{
					GBHR[k+1]=GBHR[k];
				}
				if(BRANCH_OUTCOME_ACTUAL[i] == "t")
				{
					GBHR[0] = 1;
				}
				else
				{
					GBHR[0] = 0;
				}
			}
		}
	}
	void makeprediction(float branchindex, int index_i,int indicate)
	{
		string predicted_outcome;
		PREDICTIONS++;
		if(TYPE=="bimodal"||(TYPE=="hybrid" && indicate==1))
		{
			if(PREDICTIONTABLE_BIMODAL[branchindex]>=2)
			{	
				predicted_outcome="t";
				OUTCOME_BIMODAL[index_i]=predicted_outcome;
				if(predicted_outcome==BRANCH_OUTCOME_ACTUAL[index_i])
				{
					CORRECTPREDICTIONS++;
				}
				else
				{
					MISSPREDICTIONS++;
				}
			}
			else
			{
				predicted_outcome="n";
				OUTCOME_BIMODAL[index_i]=predicted_outcome;
				if(predicted_outcome==BRANCH_OUTCOME_ACTUAL[index_i])
				{
					CORRECTPREDICTIONS++;
				}
				else
				{
					MISSPREDICTIONS++;
				}
			}
		}
		else if(TYPE=="gshare"||(TYPE=="hybrid" && indicate==2))
		{
			if(PREDICTIONTABLE_GSHARE[branchindex]>=2)
			{	
				predicted_outcome="t";
				OUTCOME_GSHARE[index_i]=predicted_outcome;
				if(predicted_outcome==BRANCH_OUTCOME_ACTUAL[index_i])
				{
					CORRECTPREDICTIONS++;
				}
				else
				{
					MISSPREDICTIONS++;
				}
			}
			else
			{
				predicted_outcome="n";
				OUTCOME_GSHARE[index_i]=predicted_outcome;
				if(predicted_outcome==BRANCH_OUTCOME_ACTUAL[index_i])
				{
					CORRECTPREDICTIONS++;
				}
				else
				{
					MISSPREDICTIONS++;
				}
			}
		}
	}
	void counterupdate(float branchindex, int index_i,int indicate)
	{
		if(TYPE=="bimodal"||(TYPE=="hybrid" && indicate==1))
		{
			if(BRANCH_OUTCOME_ACTUAL[index_i]=="t")
			{
				if(PREDICTIONTABLE_BIMODAL[branchindex]<3)
				{
					PREDICTIONTABLE_BIMODAL[branchindex]++;
				}
			}
			else
			{
				if(PREDICTIONTABLE_BIMODAL[branchindex]>0)
				{
					PREDICTIONTABLE_BIMODAL[branchindex]--;
				}
			}
		}
		else if(TYPE=="gshare"||(TYPE=="hybrid" && indicate==2))
		{
			if(BRANCH_OUTCOME_ACTUAL[index_i]=="t")
			{
				if(PREDICTIONTABLE_GSHARE[branchindex]<3)
				{
					PREDICTIONTABLE_GSHARE[branchindex]++;
				}
			}
			else
			{
				if(PREDICTIONTABLE_GSHARE[branchindex]>0)
				{
					PREDICTIONTABLE_GSHARE[branchindex]--;
				}
			}
		}
	}
	void lruinit()
	{
		for(int i=0;i<BTB_NUMBEROFSETS;i++)
		{
			for(int j=0;j<BTB_ASSOC;j++)
			{
				BTB_LRUCOUNT[i][j]=BTB_ASSOC-j-1;
			}
		}
	}
	int BTBop(float branchindex, int index_i)
	{
		if(BTB_ASSOC!=0)
		{
			int temp=0;
			int tempmax=0;
			int tempindex=0;
			int invalid_read=0;
			for(int j=0;j<BTB_ASSOC;j++)
			{
				if(BTB_TAG[branchindex][j]==BTB_TAGVALUE[index_i])
				{
					BTB_indicate="t";
					temp=BTB_LRUCOUNT[branchindex][j];
					for(int k=0; k<BTB_ASSOC; k++)
					{
						if(BTB_LRUCOUNT[branchindex][k]<temp)
						{
							BTB_LRUCOUNT[branchindex][k]=BTB_LRUCOUNT[branchindex][k]+1;
						}
					}
					BTB_LRUCOUNT[branchindex][j]=0;
					return 1;
				}
			}
			BTB_indicate="n";
			if(BRANCH_OUTCOME_ACTUAL[index_i]!=BTB_indicate)
			{
				BTB_MISSPREDICTIONS++;
			}
			for(int j=0;j<BTB_ASSOC;j++)
			{
				if(BTB_VALIDBITS[branchindex][j]==0)
				{
					BTB_TAG[branchindex][j]=BTB_TAGVALUE[index_i];
					BTB_VALIDBITS[branchindex][j]=1;
					temp=BTB_LRUCOUNT[branchindex][j];
					for(int k=0; k<BTB_ASSOC ; k++)
					{
						if(BTB_LRUCOUNT[branchindex][k]<temp)
						{
							BTB_LRUCOUNT[branchindex][k]=BTB_LRUCOUNT[branchindex][k]+1;
						}
					}
					BTB_LRUCOUNT[branchindex][j]=0;
					return 1;
				}
				else
				{
					invalid_read=invalid_read+1;
				}
			}
			if(invalid_read==BTB_ASSOC)
			{
				tempmax=0;
				for(int k=0;k<BTB_ASSOC;k++)
				{
					if(BTB_LRUCOUNT[branchindex][k]>tempmax)
					{
						tempmax = BTB_LRUCOUNT[branchindex][k];
						tempindex=k;
					}
				}
				BTB_TAG[branchindex][tempindex]=BTB_TAGVALUE[index_i];
				BTB_VALIDBITS[branchindex][tempindex]=1;
				for(int k=0;k<BTB_ASSOC;k++)
				{
					BTB_LRUCOUNT[branchindex][k]=BTB_LRUCOUNT[branchindex][k]+1;
				}
				BTB_LRUCOUNT[branchindex][tempindex]=0;
				return 1;
			}
		}
	}
};
class BIMODAL:public PREDICTOR
{
	public:
	BIMODAL(int a,int b,int c,string d,vector<string> e,int f,string g,string h)
	{
		this->M_BIMODAL=a;
		this->BTB_SIZE=b;
		this->BTB_ASSOC=c;
		this->TRACEFILE=d;
		this->TRACES=e;
		this->NUMBEROFTRACES=f;
		this->TYPE=g;
		this->FIRSTARG=h;
		PREDICTIONTABLE_BIMODAL.resize((pow(2,M_BIMODAL)),2);
		PC.resize(NUMBEROFTRACES);
		BRANCH_OUTCOME_ACTUAL.resize(NUMBEROFTRACES);
		PC_BINARY.resize(NUMBEROFTRACES);
		INDEX_BIMODAL_BINARY.resize(NUMBEROFTRACES);
		INDEX_BIMODAL_DECIMAL.resize(NUMBEROFTRACES);
		PREDICTIONS=0.0;
		CORRECTPREDICTIONS=0.0;
		MISSPREDICTIONS=0.0;
		MISSPREDICTION_RATE=0.0;
		OUTCOME_BIMODAL.resize(NUMBEROFTRACES);
		//*********BTB variables***********//
		if(BTB_ASSOC!=0)
		{
			BTB_NUMBEROFSETS = (BTB_SIZE)/(BTB_ASSOC*4);
		}
		else
		{
			BTB_NUMBEROFSETS = 0;
		}
		BTB_TAG.resize(BTB_NUMBEROFSETS,vector <string>(BTB_ASSOC , "" ) );
		BTB_VALIDBITS.resize(BTB_NUMBEROFSETS,vector <int>(BTB_ASSOC , 0 ) );
		BTB_LRUCOUNT.resize(BTB_NUMBEROFSETS,vector <int>(BTB_ASSOC , 0 ) );
		BTB_TAGVALUE.resize(NUMBEROFTRACES);
		BTB_INDEX.resize(NUMBEROFTRACES);
		BTB_DECIMALINDEX.resize(NUMBEROFTRACES);         
		//int BRANCH_PREDICTIONS;            
		BTB_MISSPREDICTIONS=0;
	}
	void bimodalop()
	{
		lruinit();
		for(int i=0;i<NUMBEROFTRACES;i++)
		{
			if(BTB_ASSOC!=0)
			{
				BTBop(BTB_DECIMALINDEX[i],i);
				if(BTB_indicate=="t")
				{
					makeprediction(INDEX_BIMODAL_DECIMAL[i],i,0);
					counterupdate(INDEX_BIMODAL_DECIMAL[i],i,0);
				}
			}
			else
			{
				makeprediction(INDEX_BIMODAL_DECIMAL[i],i,0);
				counterupdate(INDEX_BIMODAL_DECIMAL[i],i,0);
			}
		}
		cout.precision(2);
		 MISSPREDICTION_RATE = ((MISSPREDICTIONS+BTB_MISSPREDICTIONS)/(NUMBEROFTRACES))*100.0;
	     cout<<"COMMAND"<<endl;
	     cout<<FIRSTARG<<" "<<TYPE<<" "<<M_BIMODAL<<" "<<BTB_SIZE<<" "<<BTB_ASSOC<<" "<<TRACEFILE<<endl;
	     cout<<"OUTPUT"<<endl;
		 if(BTB_ASSOC!=0)
	    {
	     	cout<<"size of BTB: "<<"	 "<<BTB_SIZE<<endl;
	     	cout<<"number of branches:"<<"	 "<<NUMBEROFTRACES<<endl;
         	cout<<"number of predictions from branch predictor:	"  <<dec<<int(PREDICTIONS)<<endl;
	     	cout<<"number of mispredictions from branch predictor:	"<<int(MISSPREDICTIONS)<<endl;
	     	cout<<"number of branches miss in BTB and taken:	 "<<BTB_MISSPREDICTIONS<<endl;
	     	cout<<"total mispredictions:	"<<int((BTB_MISSPREDICTIONS + MISSPREDICTIONS))<<endl;
	     	cout<<"misprediction rate:	"<<fixed<<MISSPREDICTION_RATE<<"%"<<endl;
	     	cout<<endl;
	     	cout<<"FINAL BTB CONTENTS"<<endl;
	     	for(int i=0;i<BTB_NUMBEROFSETS;i++)
	     	{
	     		cout<<"set "<<dec<<i<<":	";
	     		for(int j=0;j<BTB_ASSOC;j++)
	     		{
	     			int X = stoi(BTB_TAG[i][j],NULL,2);
	     			cout<<hex<<X<<"  	";
	     		}
	     	 cout<<endl;
	     	}
	     	cout<<endl;
	     	cout<< "FINAL BIMODAL CONTENTS"<<endl;
	     	for(int i=0;i<pow(2,M_BIMODAL);i++)
	     	{
	     		cout<<dec<<i<< "	" <<PREDICTIONTABLE_BIMODAL[i]<<endl;
	     	}
	    }
		else
		{
			cout<<"number of predictions: "<<dec<< int(PREDICTIONS)<<endl;
			cout<<"number of mispredictions: "<<int(MISSPREDICTIONS)<<endl;
			cout<<"misprediction rate: "<<fixed<<MISSPREDICTION_RATE<<"%"<<endl;
			cout<<" FINAL BIMODAL CONTENTS"<<endl;
			for(int i=0;i<pow(2,M_BIMODAL);i++)
			{
				cout<<i<<"    "<<PREDICTIONTABLE_BIMODAL[i]<<endl;
			}
		}
	}
};
class GSHARE:public PREDICTOR
{
	public:
	GSHARE(int a,int b,int c,int d,string e,vector<string> f,int g,string h,string i)
	{
		this->M_GSHARE=a;
		this->N_GSHARE=b;
		this->BTB_SIZE=c;
		this->BTB_ASSOC=d;
		this->TRACEFILE=e;
		this->TRACES=f;
		this->NUMBEROFTRACES=g;
		this->TYPE=h;
		this->FIRSTARG=i;
		PREDICTIONTABLE_GSHARE.resize((pow(2,M_GSHARE)),2);
		PC.resize(NUMBEROFTRACES);
		BRANCH_OUTCOME_ACTUAL.resize(NUMBEROFTRACES);
		PC_BINARY.resize(NUMBEROFTRACES);
		INDEX_GSHARE_BINARY.resize(NUMBEROFTRACES);
		INDEX_GSHARE_DECIMAL.resize(NUMBEROFTRACES);
		PREDICTIONS=0.0;
		CORRECTPREDICTIONS=0.0;
		MISSPREDICTIONS=0.0;
		MISSPREDICTION_RATE=0.0;
		GBHR.resize(N_GSHARE,0);
		OUTCOME_GSHARE.resize(NUMBEROFTRACES);
		//*********BTB variables***********//
		if(BTB_ASSOC!=0)
		{
			BTB_NUMBEROFSETS = (BTB_SIZE)/(BTB_ASSOC*4);
		}
		else
		{
			BTB_NUMBEROFSETS = 0;
		}
		BTB_TAG.resize(BTB_NUMBEROFSETS,vector <string>(BTB_ASSOC , "" ) );
		BTB_VALIDBITS.resize(BTB_NUMBEROFSETS,vector <int>(BTB_ASSOC , 0 ) );
		BTB_LRUCOUNT.resize(BTB_NUMBEROFSETS,vector <int>(BTB_ASSOC , 0 ) );
		BTB_TAGVALUE.resize(NUMBEROFTRACES);
		BTB_INDEX.resize(NUMBEROFTRACES);
		BTB_DECIMALINDEX.resize(NUMBEROFTRACES);         
		//int BRANCH_PREDICTIONS;            
		BTB_MISSPREDICTIONS=0;
	}
	void gop()
	{
		if(BTB_ASSOC!=0)
		{
			for(int i=0;i<BTB_NUMBEROFSETS;i++)
			{
				for(int j=0;j<BTB_ASSOC;j++)
				{
					BTB_TAG[i][j]="";
					BTB_LRUCOUNT[i][j]=0;
					BTB_VALIDBITS[i][j]=0;
				}
			}
			BTB_MISSPREDICTIONS=0;
			BTB_indicate="";
		}
		lruinit();
		for(int i=0;i<NUMBEROFTRACES;i++)
		{
			if(BTB_ASSOC!=0)
			{
				BTBop(BTB_DECIMALINDEX[i],i);
				if(BTB_indicate=="t")
				{
					makeprediction(INDEX_GSHARE_DECIMAL[i],i,0);
					counterupdate(INDEX_GSHARE_DECIMAL[i],i,0);
				}
			}
			else
			{
				makeprediction(INDEX_GSHARE_DECIMAL[i],i,0);
				counterupdate(INDEX_GSHARE_DECIMAL[i],i,0);
			}
		}
		cout.precision(2);
		 MISSPREDICTION_RATE = ((MISSPREDICTIONS+BTB_MISSPREDICTIONS)/(NUMBEROFTRACES))*100.0;
	     cout<<"COMMAND"<<endl;
	     cout<<FIRSTARG<<" "<<TYPE<<" "<<M_GSHARE<<" "<<N_GSHARE<<" "<<BTB_SIZE<<" "<<BTB_ASSOC<<" "<<TRACEFILE<<endl;
	     cout<<"OUTPUT"<<endl;
		if(BTB_ASSOC!=0)
	    {
	     	cout<<"size of BTB: "<<"	 "<<BTB_SIZE<<endl;
	     	cout<<"number of branches:"<<"	 "<<NUMBEROFTRACES<<endl;
         	cout<<"number of predictions from branch predictor:	"  <<dec<<int(PREDICTIONS)<<endl;
	     	cout<<"number of mispredictions from branch predictor:	"<<int(MISSPREDICTIONS)<<endl;
	     	cout<<"number of branches miss in BTB and taken:	 "<<BTB_MISSPREDICTIONS<<endl;
	     	cout<<"total mispredictions:	"<<int((BTB_MISSPREDICTIONS + MISSPREDICTIONS))<<endl;
	     	cout<<"misprediction rate:	"<<fixed<<MISSPREDICTION_RATE<<"%"<<endl;
	     	cout<<endl;
	     	cout<<"FINAL BTB CONTENTS"<<endl;
	     	for(int i=0;i<BTB_NUMBEROFSETS;i++)
	     	{
	     		cout<<"set "<<dec<<i<<":	";
	     		for(int j=0;j<BTB_ASSOC;j++)
	     		{
	     			int X = stoi(BTB_TAG[i][j],NULL,2);
	     			cout<<hex<<X<<"  	";
	     		}
	     	 cout<<endl;
	     	}
	     	cout<<endl;
	     	cout<< "FINAL GSHARE CONTENTS"<<endl;
	     	for(int i=0;i<pow(2,M_GSHARE);i++)
	     	{
	     		cout<<dec<<i<< "	" <<PREDICTIONTABLE_GSHARE[i]<<endl;
	     	}
	    }
		else
		{
			 cout<<"number of predictions: "<<dec<< int(PREDICTIONS)<<endl;
			 cout<<"number of mispredictions: "<<int(MISSPREDICTIONS)<<endl;
			 cout<<"misprediction rate: "<<fixed<<MISSPREDICTION_RATE<<"%"<<endl;
			 cout<< "FINAL GSHARE CONTENTS"<<endl;
			 for(int i=0;i<pow(2,M_GSHARE);i++)
			 {
				cout<<i<< "    " <<PREDICTIONTABLE_GSHARE[i]<<endl;
			 }
		}
	}
};
class HYBRID:public PREDICTOR
{
	public:
	vector<int> PREDICTIONTABLE_HYBRID;
	float PREDICTIONS_HYBRID;
	float MISSPREDICTIONS_HYBRID;
	float MISSPREDICTION_RATE_HYBRID;
	HYBRID(int a,int b,int c,int d,int e,int f,string g,vector<string> h,int i,string j,string k)
	{
		this->K_HYBRID=a;
		this->M_GSHARE=b;
		this->N_GSHARE=c;
		this->M_BIMODAL=d;
		this->BTB_SIZE=e;
		this->BTB_ASSOC=f;
		this->TRACEFILE=g;
		this->TRACES=h;
		this->NUMBEROFTRACES=i;
		this->TYPE=j;
		this->FIRSTARG=k;
		PC.resize(NUMBEROFTRACES);
		BRANCH_OUTCOME_ACTUAL.resize(NUMBEROFTRACES);
		PC_BINARY.resize(NUMBEROFTRACES);
		PREDICTIONS=0.0;
		CORRECTPREDICTIONS=0.0;
		MISSPREDICTIONS=0.0;
		MISSPREDICTION_RATE=0.0;
		PREDICTIONTABLE_BIMODAL.resize((pow(2,M_BIMODAL)),2);
		INDEX_BIMODAL_BINARY.resize(NUMBEROFTRACES);
		INDEX_BIMODAL_DECIMAL.resize(NUMBEROFTRACES);
		PREDICTIONTABLE_GSHARE.resize((pow(2,M_GSHARE)),2);
		INDEX_GSHARE_BINARY.resize(NUMBEROFTRACES);
		INDEX_GSHARE_DECIMAL.resize(NUMBEROFTRACES);
		GBHR.resize(N_GSHARE,0);
		PREDICTIONTABLE_HYBRID.resize((pow(2,K_HYBRID)),1);
		PREDICTIONS_HYBRID=0.0;
		MISSPREDICTIONS_HYBRID=0.0;
		MISSPREDICTION_RATE_HYBRID=0.0;
		OUTCOME_BIMODAL.resize(NUMBEROFTRACES);
		OUTCOME_GSHARE.resize(NUMBEROFTRACES);
		INDEX_HYBRID_BINARY.resize(NUMBEROFTRACES);
		INDEX_HYBRID_DECIMAL.resize(NUMBEROFTRACES);
		//*********BTB variables***********//
		if(BTB_ASSOC!=0)
		{
			BTB_NUMBEROFSETS = (BTB_SIZE)/(BTB_ASSOC*4);
		}
		else
		{
			BTB_NUMBEROFSETS = 0;
		}
		BTB_TAG.resize(BTB_NUMBEROFSETS,vector <string>(BTB_ASSOC , "" ) );
		BTB_VALIDBITS.resize(BTB_NUMBEROFSETS,vector <int>(BTB_ASSOC , 0 ) );
		BTB_LRUCOUNT.resize(BTB_NUMBEROFSETS,vector <int>(BTB_ASSOC , 0 ) );
		BTB_TAGVALUE.resize(NUMBEROFTRACES);
		BTB_INDEX.resize(NUMBEROFTRACES);
		BTB_DECIMALINDEX.resize(NUMBEROFTRACES);         
		//int BRANCH_PREDICTIONS;            
		BTB_MISSPREDICTIONS=0;
	}
	void hop()
	{
		//decodelogic();
		string predicted_outcome_hybrid;
		int tempvalue_i=0;
		for(int i=0;i<NUMBEROFTRACES;i++)
		{	tempvalue_i=INDEX_HYBRID_DECIMAL[i];
			makeprediction(INDEX_BIMODAL_DECIMAL[i],i,1);
			makeprediction(INDEX_GSHARE_DECIMAL[i],i,2);
			if(PREDICTIONTABLE_HYBRID[tempvalue_i]>=2)
			{
				predicted_outcome_hybrid=OUTCOME_GSHARE[i];
				counterupdate(INDEX_GSHARE_DECIMAL[i],i,2);
			}
			else
			{
				predicted_outcome_hybrid=OUTCOME_BIMODAL[i];
				counterupdate(INDEX_BIMODAL_DECIMAL[i],i,1);
			}
			if(OUTCOME_GSHARE[i]==BRANCH_OUTCOME_ACTUAL[i]&&OUTCOME_BIMODAL[i]!=BRANCH_OUTCOME_ACTUAL[i])
			{
				if(PREDICTIONTABLE_HYBRID[tempvalue_i]<3)
				{
					PREDICTIONTABLE_HYBRID[tempvalue_i]++;
				}
			}
			else if(OUTCOME_GSHARE[i]!=BRANCH_OUTCOME_ACTUAL[i]&&OUTCOME_BIMODAL[i]==BRANCH_OUTCOME_ACTUAL[i])
			{
				if(PREDICTIONTABLE_HYBRID[tempvalue_i]>0)
				{
					PREDICTIONTABLE_HYBRID[tempvalue_i]--;
				}
			}
			if(predicted_outcome_hybrid==BRANCH_OUTCOME_ACTUAL[i])
			{
				PREDICTIONS_HYBRID++;
			}
			else
			{
				MISSPREDICTIONS_HYBRID++;
			}
		}
		cout.precision(2);
		 MISSPREDICTION_RATE_HYBRID = ((MISSPREDICTIONS_HYBRID)/(NUMBEROFTRACES))*100.0;
		 cout<<"COMMAND"<<endl;
	     cout<<FIRSTARG<<" "<<TYPE<<" "<<K_HYBRID<<" "<<M_GSHARE<<" "<<N_GSHARE<<" "<<M_BIMODAL<<" "<<BTB_SIZE<<" "<<BTB_ASSOC<<" "<<TRACEFILE<<endl;
	     cout<<"OUTPUT"<<endl;
	     cout<<"number of predictions: "<<dec<< int(NUMBEROFTRACES)<<endl;
	     cout<<"number of mispredictions: "<<int(MISSPREDICTIONS_HYBRID)<<endl;
	     cout<<"misprediction rate: "<<fixed<<MISSPREDICTION_RATE_HYBRID<<"%"<<endl;
	     cout<<" FINAL CHOOSER CONTENTS"<<endl;
	     for(int i=0;i<pow(2,K_HYBRID);i++)
	     {
	     	cout<<i<<"    "<<PREDICTIONTABLE_HYBRID[i]<<endl;
	     }
	     cout<< "FINAL GSHARE CONTENTS"<<endl;
	     for(int i=0;i<pow(2,M_GSHARE);i++)
	     {
	     	cout<<i<< "    " <<PREDICTIONTABLE_GSHARE[i]<<endl;
	     }
	     cout<<" FINAL BIMODAL CONTENTS"<<endl;
	     for(int i=0;i<pow(2,M_BIMODAL);i++)
	     {
	     	cout<<i<<"    "<<PREDICTIONTABLE_BIMODAL[i]<<endl;
	     }
	}
};
int main(int argc, char *argv[])
{
	int m_bimodal;
	int BTB_size;
	int BTB_assoc;
	string tracefile;
	string type_of_predictor=argv[1];
	vector<string> traces;
	string line;
	int n=0;
	int m_gshare;
	int n_gshare;
	int k_hybrid;
	string firstarg=argv[0];
	if(type_of_predictor=="bimodal")
	{
		m_bimodal=atoi(argv[2]);
		BTB_size=atoi(argv[3]);
		BTB_assoc=atoi(argv[4]);
		tracefile=argv[5];
		ifstream trace(argv[5]);
		while (getline(trace, line)) // storing trace file PC values into vector traces
		{
			traces.push_back(line);
			n=n+1;
		}
		BIMODAL bi(m_bimodal,BTB_size,BTB_assoc,tracefile,traces,n,type_of_predictor,firstarg);
		bi.decodelogic();
		bi.bimodalop();
		
	}
	if(type_of_predictor=="gshare")
	{
		m_gshare=atoi(argv[2]);
		n_gshare=atoi(argv[3]);
		BTB_size=atoi(argv[4]);
		BTB_assoc=atoi(argv[5]);
		tracefile=argv[6];
		ifstream trace(argv[6]);
		while (getline(trace, line)) // storing trace file PC values into vector traces
		{
			traces.push_back(line);
			n=n+1;
		}
		GSHARE g(m_gshare,n_gshare,BTB_size,BTB_assoc,tracefile,traces,n,type_of_predictor,firstarg);
		g.decodelogic();
		g.gop();
	}
	if(type_of_predictor=="hybrid")
	{
		k_hybrid=atoi(argv[2]);
		m_gshare=atoi(argv[3]);
		n_gshare=atoi(argv[4]);
		m_bimodal=atoi(argv[5]);
		BTB_size=atoi(argv[6]);
		BTB_assoc=atoi(argv[7]);
		tracefile=argv[8];
		ifstream trace(argv[8]);
		while (getline(trace, line)) // storing trace file PC values into vector traces
		{
			traces.push_back(line);
			n=n+1;
		}
		HYBRID h(k_hybrid,m_gshare,n_gshare,m_bimodal,BTB_size,BTB_assoc,tracefile,traces,n,type_of_predictor,firstarg);
		h.decodelogic();
		h.hop();
	}
}