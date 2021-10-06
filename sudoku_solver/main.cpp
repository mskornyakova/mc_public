#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include <memory.h>

using namespace std;

class MCMap;

list<MCMap*> g_allSolutions;

constexpr int BIG_VALUE = 10000;

// - - - - - - - - - - - - - - - - - - - - - - - - - -
bool isValidSudoku(const char* sud)
{
    for(int k = 0; k < 81; k++)
        if (sud[k] != ' ' && ((int)sud[k] > 57 || (int)sud[k]< 49))
        {
            cout<<"Wrang initial sudoku!, key = "<<k<<" val = "<<sud[k]<<", code "<<(int)sud[k]<<endl;
            return false;
        }
     return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - -
void printSudoku(const char* sud)
{
    cout<<" S U D O K U"<<endl;
    for(int i = 0; i < 81; i++)
    {
        cout<<sud[i]<< " ";
        if ((i+1) % 9 == 0)
            cout<<endl;
    }
    cout<<endl;
}



// - - - - - - - - - - - - - - - - - - - - - - - - - -
class MCMap : public unordered_map<int, string>
{
    typedef pair<int,string> pairPosDopus;
    typedef unordered_map<int, string>::iterator MCMapIterator;
    vector<MCMapIterator> m_min;
    size_t m_minCount;
    char * m_sud;
public:
   MCMap(char* sud): m_minCount(BIG_VALUE)
   {
       m_sud = new char[82];
       memcpy(m_sud, sud, 82);
   }
   MCMap(const MCMap& map, int key, char val): m_minCount(BIG_VALUE)
   {
       m_sud = new char[82];
       memcpy(m_sud, map.getSud(), 82);
       m_sud[key] = val;
       for (auto it = map.begin(); it != map.end(); it++)
       {
           if(it->first == key)
               continue;
           insert(pairPosDopus(it->first,it->second));
       }
   }

   ~MCMap()
   {
       m_min.clear();
       delete[] m_sud;
       clear();
   }

   //-----
   const char * getSud() const
   {
       return m_sud;
   }

   //-----
   // k - позиция в матрице судоку [9][9]->[81]
   // dop строка допустимых значений
   // если в строке, столбце или блоке m_sud уже есть число из dop, то данное число удаляется из списка допустимых
   // на выходе, dop - есть строка допустимых чисел для клетки k
   void dopus(int k, string& dop)
   {
       int i = k / 9;
       int j = k % 9;
       int ib = i / 3;
       int jb = j / 3;
       int ind_b = ib*9*3 + jb*3;
       int io = i*9;
       for (int l = 0; l < 9; l++)
       {
           size_t pos = dop.find(m_sud[io + l]);
           if (std::string::npos != pos)
               dop.erase(pos,1);

           pos = dop.find(m_sud[j + l*9]);
           if (std::string::npos != pos)
               dop.erase(pos,1);

           pos = dop.find(m_sud[ind_b + (l/3)*9 + l % 3]);
           if (std::string::npos != pos)
               dop.erase(pos,1);
       }
   }

   //-----
   void initMap()
   {
        clear();
        for (int k = 0; k < 81; k++)
        {
            if(m_sud[k] == ' ')
            {
                string dop("123456789");
                dopus(k, dop);
                pair<MCMapIterator, bool> isresult = insert(pairPosDopus(k, dop));
                if (dop.size() < m_minCount)
                {
                    m_min.clear();
                    m_min.push_back(isresult.first);
                    m_minCount = dop.size();
                }
                else if (dop.size() == m_minCount)
                    m_min.push_back(isresult.first);
            }
        }
    }

   //-----
   bool solve()
   {
       m_minCount = BIG_VALUE;
       m_min.clear();
       if (empty())
           return true;

       for(MCMapIterator it = begin(); it != end(); it++)
       {
           dopus(it->first, it->second);
           if (it->second.size() < m_minCount)
           {
               m_min.clear();
               m_min.push_back(it);
               m_minCount = it->second.size();
           }
           else if (it->second.size() == m_minCount)
               m_min.push_back(it);
       }

       return chooseElem();
   }

   //-----
   bool chooseElem()
   {
       if (0 == m_minCount){
         //  cout<<" unresolved!!! "<<endl;
           return false;
       }
       else if (1 == m_minCount){ // для клетки существует единственное допустимое число
         //  cout<<" single possible element value"<<endl;
           for(const auto& it : m_min){
               m_sud[it->first]=it->second[0];
               erase(it->first);
           }
           return solve();
       }
       else{
           for (size_t i = 1; i < m_minCount; i++){
               if (g_allSolutions.size() > 100){
                   cout<<"Too many solutions for initial state"<<endl;
                   return false;
               }
               MCMap* map = new MCMap(*this, m_min.at(0)->first, m_min.at(0)->second[i]);
               if (!map->solve()){
                //   cout<<"delete 2"<<endl;
                   delete map;
               }
               else{
                   g_allSolutions.push_back(map);
//                   cout<<"solution count "<<g_allSolutions.size()<<endl;
                }
            }

           m_sud[m_min.at(0)->first]=m_min.at(0)->second[0];
           erase(m_min.at(0)->first);
           return solve();
       }
       return true;
   }
};


// - - - - - - - - - - - - - - - - - - - - - - - - - -
int main(int argc, char *argv[])
{
    cout << "Sudoku solver" << endl;
//    char sud[82] = " 1 25 4 9         2    1 6 9    3            5              3          6    7    ";  // > 2000000
//    char sud[82] = " 84  61 5        75  8 4  2    1 9     76      75  8 1         9  2 5 14   48    ";  // 1
//    char sud[82] = "  53     8      2  7  1 5  4    53   1  7   6  32   8  6 5    9  4    3      97  ";  // 18

    char sud[82]="      59       287          9 32  5  82576  3 7  8       8   46   9  3  32  6    ";   // 1
//    char sud[82] = "    2    137 6       4   1  1 7   23 6 38  4 42     6           4 83 9  6   9    "; // 33
    //gets(sud);

    printSudoku(sud);
    if (!isValidSudoku(sud))
        return 0;

    MCMap* solver = new MCMap(sud);
    solver->initMap();
    if (solver->chooseElem())
        g_allSolutions.push_back(solver);
    else
        delete solver;

    cout<<"Solution count = "<<g_allSolutions.size()<<endl;

    int k = 0;
    for (auto it = g_allSolutions.begin(); it != g_allSolutions.end(); it++)
    {
        cout<<" "<<++k;
        printSudoku((*it)->getSud());
        delete *it;
    }

    g_allSolutions.clear();

    return 0;
}
