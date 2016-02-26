#include<iostream>
#include<vector>
#include<fstream>
#include<math.h>
#include<string>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

class Compression
{
    public:
        long arraysize ;
        long Dsize;
        long primes[6]; 
        string *dictionary;
        int m;
		long hashe;
		//long test=0;
		int lookup[6];
	
        Compression(int n)
        {
			Dsize =0;
			primes[0]=200063;
			primes[1]=400031;
			primes[2]=999983;
			primes[3]=10000019;
			primes[4]=100000007;
			primes[5]=999999883;
			lookup[0]=3;
			lookup[1]=4;
			lookup[2]=4;
			lookup[3]=4;
			lookup[4]=5;
			lookup[5]=6;
            arraysize = primes[n];
            dictionary = new string[arraysize];
            string a;
            m =n;
			int plup;
            for (int i = 0; i<254; i++)
            {
                string a;
                a = (char)i;
                dictionary[i]=a;
                Dsize++;
            }
			a=(char)254;
            for (long i = 254; i<arraysize; i++)
            {
                dictionary[i]=a;
            }
        }
		string decimal_base(long x)
		{
			string a,b;
			a="";
			int y,v;
			unsigned long long z;
			while(x>0)
			{
				y=x%62;
				v=y+65;
				b=(char)v;
				a=b+a;
				x=x/62;
			}
			return a;
		}
	
	
        long hashing_fn(string x,int nr)
        {
            long k=0;
			long hashsize = primes[nr];
            for(int i=0;i<x.length();i--)
            {
				if(x[i]>=0)
                {
					k = (x[i]+256*k)%hashsize;
				}
				if(x[i]<0)
				{
					k = (256+x[i]+256*k)%hashsize;
				}
            }
            return k;
        }

		void add_Dictionary(string x)
		{
			string a;
            long y = hashing_fn(x,m);
            a = (char)254;
            bool foundfree=false;
            long k=0;
			hashe = primes[m];
            while(k<(primes[m]+1)/2 and foundfree==false)
            {
                if(dictionary[(y+(k*k))%hashe]==a)
                {
                    dictionary[(y+k*k)%hashe]=x;
                    foundfree = true;
                    Dsize++;
                }
                else
                {
                    k++;
                }
            }
        }

        void printer()
        {
            for(int i=0;i<300  ;i++)
            {
                cout<<dictionary[i];
            }
        }

        bool isFull()
        {
			//test++;
			//cout<<"a"<<Dsize<<test<<endl;
            if (Dsize==arraysize)
            {
                return true;
            }
            else 
			{return false;}
        }

        bool find_Dict(string x)
        {
            long y = hashing_fn(x,m);
            if(dictionary[y]==x)
            {
                return true;
            }
            else
            {
                //if found before true else false
                bool found =false;
                long j=1;
				string b;
				b = (char)254;
                while(j<(primes[m]+1)/2 and found == false) //while j/\y probe
                {
						if(dictionary[(y+j*j)%primes[m]]==b)
                        {
                            j=primes[m];
                        }
                        else if(dictionary[(y+j*j)%primes[m]]==x)
                        {
                            found = true;
                        }
                        else
                        {
                            j++;
                        }
                }
                if(found)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
			
        }

    	void LZWComp(char str[],char str2[],int n)
    	{
		
			/*string w;
			char k;
			w is a empty string;
			while (data can be read)
			{
    			k = read a character;
    			if (dictionary contains w+k)
    			{
					w = w+k;
    			}
    			else
   	 			{
					output w;
					add w+k to dictionary;
					w = k;
	    		}	
			}
			output k ;*/
		
			FILE * fp;
			FILE * fp2;
			string w;
			
			if((fp=fopen(str,"r")) == NULL)
			{
				cout<<"Cannot open file"<<endl;
				return;
			}
	
			if((fp2=fopen(str2,"w")) == NULL)
			{
				cout<<"Cannot open file"<<endl;
				return;
			}
			
			char k;
			char l;
			if(fscanf(fp,"%c",&l)!=EOF)
			{
				w = l;
			}
			fprintf(fp2, "%d", n);
			while(fscanf(fp,"%c",&k)!=EOF)
			{
				//cout<<Dsize<<endl;
        	    if (find_Dict(w+k)) //find if w+k exists in dictionary
   	    	    {
   	    	    	w = w+k;
				}				
        	    else
        	    {	
    		       if(isFull())
        	       {
						//cout<<"MOOO"<<endl;
    					fclose(fp);
						fclose(fp2);
						fp =NULL;
						fp2 = NULL;
						delete fp;
						delete fp2;
						//cout<<"MOOO"<<endl;
        	            Dsize=0;
        	            arraysize = primes[n+1];
						//cout<<arraysize<<endl;
						string *newdictionary;
						newdictionary = new string[arraysize];
        	            string a;
        	            m =n+1;
        	            for (int i = 0; i<254; i++)
        	            {
        	                newdictionary[i]=dictionary[i];
        	                Dsize++;
							//cout<<Dsize<<endl;
        	            }
						a=(char)254;
        	            for (long i = 254; i<arraysize; i++)
        	            {
        	                newdictionary[i]=a;
        	            }
						dictionary = newdictionary;
						//cout<<dictionary[1000]<<endl;
        	            LZWComp(str,str2,n+1);
        	            return;
        	            //start again with increased n.
        	       }
     		       else
        	       {
						long p = hashing_fn(w,n);
				    	if(dictionary[p]==w)
        	    		{}      				
						else
       					{
        	        		bool found = false;
        	        		long j=1;
							long hashsize = primes[n];
        	   				while(j<(hashsize+1)/2 and found == false) //while j/\y probe
        	        		{
								long r = (p+j*j)%hashsize;
	    	                   	if(dictionary[r]==w)
        	               		{
									p=r;
									found =true;
        	                	}
        	                	else
        	                	{
        	                    	j++;
        	               		}
        	        		}
						}
						string pp;
						pp=decimal_base(p);
						string q=pp;
        	            int numberofdigits;
        	            numberofdigits=pp.length();
						string ze;
						ze="A";
        	            while(numberofdigits<lookup[n])
        	            {
							fprintf(fp2, "%s", ze.c_str());
        	                numberofdigits++;
        	            }
						fprintf(fp2, "%s", pp.c_str());	  	           	
						add_Dictionary(w+k);//add (w+k) to the dictionary;
						w = k;				
        	       }
   	    		}
    		}
			long p = hashing_fn(w,n);
			if(dictionary[p]==w)
        	{}
       		else
       		{
        		bool found = false;
      			long j=1;
        	   	while(j<(primes[n]+1)/2 and found == false) //while j/\y probe
        	    {
        	    	if(dictionary[(p+j*j)%primes[n]]==w)
        	        {
        	            p=(p+j*j)%primes[n];
						found =true;
        	        }
        	        else
        	        {
        	          	j++;
        	  		}
        	   	}
			}
						string pp;
						pp=decimal_base(p);
						string q=pp;
        	            int numberofdigits;
        	            numberofdigits=pp.length();
						string ze;
						ze="A";
						while(numberofdigits<lookup[n])
        	            {
							fprintf(fp2, "%s", ze.c_str());
        	                numberofdigits++;
        	            }
						fprintf(fp2, "%s", pp.c_str());
			fclose(fp);
		}	

    	void LZWComp2(char str[], char str2[])
    	{	
        	LZWComp(str,str2,m);
    	}	

		void LZWDecomp(char str[], char str2[],int n)    			
		{		//read(starting from second charecter , as first we already read) a character k in blocks of n+3; // eg if n=0 pick up blocks of 3 from text
 		
			FILE * fp;
			FILE * fp2;

			if((fp=fopen(str,"r")) == NULL)
			{
				cout<<"Cannot open file1."<<endl;
				return;
			}
	
			if((fp2=fopen(str2,"w")) == NULL)
			{
				cout<<"Cannot open file2"<<endl;
				return;
			}
	
			int hashsize;
			hashsize = lookup[n];
			long k;
			long p;
			char j;
			string w;
			string nill;
			string entry;
			k=0;
			w = "";
			Dsize=0;
			arraysize = primes[n];
   	    	dictionary = new string [arraysize];
        	string a;
        	m =n;
   	    	for (int i = 0; i<254; i++)
        	{
   	    	    string a;
        	   	a=(char)i;
   	    	 	dictionary[i]=a;
            	Dsize++;
        	}	
        	for (long i = 254; i<arraysize; i++)
        	{	
            	string a;
            	a=(char)254;
            	dictionary[i]=a;
        	}		
	
			if(fscanf(fp,"%c",&j)!=EOF)
				{
				
				}		
			else
			{
				cout<<"Cannot open file2"<<endl;
				return;
			}
				
			nill = (char)254;
		  	//	w = dictionary entry for k;
			//	while ( read a character k )    
			while(fscanf(fp,"%c",&j)!=EOF)
			{
				
				
				p=j;
				k=p;
				k=k-65;
			 	for(int i=0;i<hashsize-1;i++)
				{			
					if(fscanf(fp,"%c",&j)!=EOF)
					{
						p=j;
						p=p-65;
						k = p+k*62;
					}		
					else
					{
						/*if(p==-38)
						{
							return;
						}
						else
						{
							cout<<"Cannot open file5."<<endl;
							return;
						}*/
					}  		
				}
				
				entry = dictionary[k];
				//if k exists in the dictionary
			
				if(entry==nill)
				{
					entry = w+w[0];
				}
				fprintf(fp2, "%s", entry.c_str());		
	
				// else
				if(w != "")
				{
				// entry = w+firstcharof(w)
				//output entry
	        	 //add entry to dictionary;
	        	 //w = entry;						
					add_Dictionary(w+entry[0]);								
				}        	 
				w = entry;
			}
			fclose(fp);
		
		}
		void LZWDecomp2(char str[], char str2[])
		{
			char n;
			int m;
			FILE * fp;
		
			if((fp=fopen(str,"r")) == NULL)
			{
				cout<<"Cannot open file6"<<endl;
				return;
			}
	
			if(fscanf(fp,"%c",&n)!=EOF)
			{ 
				fclose(fp);
				m=n-'0';
				LZWDecomp(str,str2,m);					
			}		
			else
			{
				cout<<"Cannot open file7."<<endl;
				return;			
			}
		}
};

int main(int argc,char **argv)
{
	if(strcmp(argv[1],"compress")==0)
	{
		Compression Comp(0);
		char str[256];
		strcpy(str,argv[2]);
		Comp.LZWComp2(str,"coded.txt");		
	}
	else
	{
		Compression comp(0);
		char str[256];
		strcpy(str,argv[2]);
		comp.LZWDecomp2(str,"decoded.txt");
	}
	//to compress
/*	cout<<"for compression press1"<<endl;
	cout<<"for decompression press2"<<endl;
	cin >> o;
	if(o==1)
	{
		Compression Comp(0);
	   	char str[256];
        cout << "Enter the name of an existing text file: ";
        cin>>str;    // get c-string
        char str2[256];
        cout << "Enter the name of the output file: ";
        cin>>str2;
		//strcpy(str,"t1.txt");
        //strcpy(str2,"t2.txt");
		Comp.LZWComp2(str,str2);		
	}
	else if(o==2)
	{
		Compression comp(0);
	   	char str[256];
        cout << "Enter the name of an existing compressed file: ";
        cin>>str;    // get c-string
        char str2[256];
        cout << "Enter the name of the output text file: ";
        cin>>str2;
		//strcpy(str,"t1.txt");
        //strcpy(str2,"t2.txt");
		comp.LZWDecomp2(str,str2);
	}*/

}
