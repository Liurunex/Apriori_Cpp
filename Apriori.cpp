#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <set>
#include <algorithm> 
#include <map>

#define ARRAY_WIDTH 100
#define ARRAY_ROW 2127
#define MIN_SUP 7
using namespace std;

int i = 0;
int r = 0;
int c = 0;
int s = 0;
int size = 0;
int print_count = 0;
bool pass = true;
string header[ARRAY_WIDTH];


/*bool 
judgeSingle(int singel, vector<int> transaction) {
  bool returnValue = false;
  for(i=0; i<transaction.size(); i++) {
    if(singel == transaction[i])
      return true;
  }
  return returnValue;
}
*/
void 
map_print(map<vector<int>, int> Lk) {
  cout << "\n" <<"Itemset: " << endl;
  map<vector<int>, int>::iterator iitt;
  for(iitt = Lk.begin(); iitt != Lk.end(); ++iitt) {
    cout << "{";
    vector<int> print = iitt->first;
    print_count = iitt->second;
    int ssize = print.size();
    for(s=0; s<ssize; s++) {
      cout << " " << print.at(s) << " ";
    }
    cout << "}" << "\t" << print_count << endl;
  }
}

bool 
has_infrequent_subset(vector<int> candiate, set<vector<int> > Lp) {
  bool returnValue = true;
  vector<int> subset;
  size = candiate.size();
  /*the k k-1-item-subset of candiate*/
  for(i=0; i<size; i++) {
    for(s=0; s<size; s++) {
      if(s != i) {
        subset.push_back(candiate[s]);
      }
    }

    set<vector<int> >::iterator it;
    it = Lp.find(subset);
    if(it == Lp.end())
      return true;    
    subset.clear();
  }
  return false;
} 

set<vector<int> >
apriori_gen(map<vector<int>, int> Lo) {
  set<vector<int> > Lp;
  map<vector<int>, int>::iterator it_tem;
  for(it_tem = Lo.begin(); it_tem != Lo.end(); ++it_tem) {
    Lp.insert(it_tem->first); 
  }
  
  set<vector<int> > C_return;
  set<vector<int> >::iterator it_1;
  set<vector<int> >::iterator it_2;
  vector<int> candiate;
  
  for(it_1 = Lp.begin(); it_1 != Lp.end(); ++it_1) {
    for(it_2 = Lp.begin(); it_2 != Lp.end(); ++it_2) {
      size = (*it_1).size();
      vector<int> i_1 = *it_1;
      vector<int> i_2 = *it_2;
      for(i=0; i<size; i++) {
        /*case 1*/
        sort(i_1.begin(), i_1.end());
        sort(i_2.begin(), i_2.end());

        if(i == size-1) {
          if(i_1.at(i) < i_2.at(i)) {
            candiate.push_back(i_1.at(i));
            candiate.push_back(i_2.at(i));
            continue;
          }
          else {
            candiate.clear();
            pass = false;
            break;
          }
        }
        /*case 2*/
        else {
          if(i_1.at(i) == i_2.at(i)) {
            candiate.push_back(i_1.at(i));
            continue;
          }
          else {
            candiate.clear();
            pass = false;
            break;
          }
        }
      }

      if(!candiate.empty()) {
        if(!has_infrequent_subset(candiate, Lp)) {
          C_return.insert(candiate);
        }
      }

      candiate.clear();
    }
  }
  return C_return;
}

vector<map<vector<int>, int> > 
Apriori(int data[ARRAY_ROW][ARRAY_WIDTH], int min_sup) {
  /*find the L1*/
  vector<map<vector<int>, int> > Total;
  map<vector<int>, int> L1;
  map<vector<int>, int> L0;
  map<vector<int>, int> Lk;

  int C1[ARRAY_WIDTH];
  for(c=0; c < ARRAY_WIDTH; c++) 
    C1[c]=0;
  for(c=0 ; c < ARRAY_WIDTH; c++) {
    for(r=0; r < ARRAY_ROW; r++) 
      C1[c] = C1[c] + data[r][c];
  }
  for(c=0; c < ARRAY_WIDTH; c++) {
    if(C1[c] >= min_sup) { 
      vector<int> midator;
      midator.push_back(c);
      L1.insert(pair<vector<int>, int>(midator, C1[c]));
    }
  }
  map_print(L1);
  /*Notice the first time add a empty into Total*/
  Total.push_back(L0);
  Total.push_back(L1);
  /*Find other L*/
  int k = 0;
  Lk = L1;
  for(k=2; !Lk.empty(); k++) {
    set<vector<int> > Ck;
    Ck = apriori_gen(Lk);
    Lk.clear();

    int count = 0; 
    vector<int> transaction;
    set<vector<int> >::iterator it;
    vector<int> tem;

    /*start the Ck iteration*/
    for(it = Ck.begin(); it != Ck.end(); ++it) {
      /*clear the reuse parameter*/
      count = 0;
      tem.clear();
      tem = *it;
      //sort(tem.begin(), tem.end());    
      /*start scan the DB*/
      for(i=0; i<ARRAY_ROW; i++) {
        for(s=0; s<ARRAY_WIDTH; s++) {
          if(data[i][s] == 1)
            transaction.push_back(s);
        }
        //sort(transaction.begin(), transaction.end());
        /*count times*/        
        if(includes(transaction.begin(), transaction.end(),
            tem.begin(), tem.end())) {         
          count++;
        }
        /* consider without function includes*/
        /*transaction.clear();
        bool judgement = true;
        int ii = 0;
        for(ii=0; ii<tem.size(); ii++) {
          judgement = judgement && judgeSingle(tem[ii], transaction);
        }
        //cout << "JUDEGEMNT: "<< judgement << endl;     
        if(judgement) {   
          count++;
        }*/

        transaction.clear();
      }
      /*judgement to add into Lk map*/
      if(count >= min_sup) {
        Lk.insert(pair<vector<int>, int>(tem,count));   
      }  
    }
    if(Lk.empty()){
      cout << "No Found" << endl;
    }
    else {
      map_print(Lk);
      Total.push_back(Lk);
    }
  }
  return Total;
}

int main(int argc, char *argv[]) {
  int array[ARRAY_ROW][ARRAY_WIDTH];
  int idx = 0;
  int ihx = 0;
  
  char *buffer = NULL;
  size_t len = 0;
  ssize_t read;
  char *ptr = NULL;
  FILE *fp;
  
  fp = fopen ("FoodMart.csv", "r");      
  if (!fp) {
      fprintf (stderr, "failed to open file for reading\n");
      return 1;
  }
  int alert_product = 1;
  while ((read = getline (&buffer, &len, fp)) != -1) {
    
    if (alert_product) {
      char *token;
      token = strtok(buffer, ",");
      while (token != NULL) {
        header[ihx] = token;
        token = strtok(NULL, ",");
        ihx++;
      }
      alert_product = 0;
      continue;
    }

    else {
      int j = 0;
          for (j = 0, ptr = buffer; j < ARRAY_WIDTH; j++, ptr++) 
            array[idx][j] = (int)strtol(ptr, &ptr, 2);
          idx++;
    }
  }
  remove(header[ARRAY_WIDTH-1].begin(), header[ARRAY_WIDTH-1].end(), '\n');
  free(buffer);
  fclose(fp);

  vector<map<vector<int>, int> > Total = Apriori(array, MIN_SUP);

  return 0;
}