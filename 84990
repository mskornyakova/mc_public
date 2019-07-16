#include <map>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
using namespace std;

// - - - - - - - - - - - - - - - - - - - -
void initContainers(vector<char>& vect, map<unsigned int,char>& _map,unsigned int size)
{
    printf("%s\n",__func__); // эквивалентно cout<<__func__<<endl;
    char value;
    for (size_t i=0; i<size; i++)
    {
        value=rand()%9+1;
        vect.push_back(value);
        value=rand()%9+1;//rand()%9+1;
        _map.insert(pair<unsigned int,int>(i,value));
    }

}

// - - - - - - - - - - - - - - - - - - - -
void printContainers(const vector<char>& vect,const map<unsigned int,char>& _map)
{
    printf("%s\n",__func__);
    size_t i=0;
    for (auto&& [key,value] : _map)
    {
        if (i<vect.size())
            printf("vector.at(%i)=%i \t\t map[%i]=%i\n", i, vect.at(i), key, value);
        else
            printf("\t\t\t map[%i]=%i\n", key, value);
        i++;
    }
    while (i<vect.size())
    {
        printf("vector.at(%i)=%i\n",i,vect.at(i));
        i++;
    }
}

// - - - - - - - - - - - - - - - - - - - -
void cutFromVector(vector<char>& vect)
{
    printf("%s\n",__func__);
    size_t count=rand()%15+1;
    cout<<"cut "<<count<<" elements from vector\n";
    if (count>=vect.size())
        vect.clear();
    else {
        // поскольку удаляется произвольное количество элементов
        // то удаление следующих подряд элементов не противоречит условию задачи
        // будем удалять count элементов с конца вектора
        auto it=vect.end();
        it -= static_cast<int>(count);
     //   printf("count=%i, vector[]=%i\n",count,*it);
        vect.erase(it,vect.end());
    }
}

// - - - - - - - - - - - - - - - - - - - -
void cutFromMap(map<unsigned int,char>& _map)
{
    printf("%s\n",__func__);
    size_t count=rand()%15+1;
    cout<<"cut "<<count<<" elements from map\n";
    if (count>=_map.size())
    {
        _map.clear();
        return;
    }

    while(count>0)
    {
        unsigned int key=static_cast<unsigned int>(rand())%(_map.size()-1);
        auto it = _map.find(key);
        if (it == _map.end())
            continue;
        _map.erase(it);
        count--;
    }

}

// - - - - - - - - - - - - - - - - - - - -
void intersectionContainers(vector<char>& vect, map<unsigned int,char>& _map)
{
    printf("%s\n",__func__);
    if (vect.size()==0 or _map.size()==0)
    {
        vect.clear();
        _map.clear();
        printf(" Intersection of vector and map not found\n");
        return;
    }
    for(char i=1; i<10; i++)
    {
        bool _del=true;
        auto itv=find(vect.begin(),vect.end(), i);
        if (itv!=vect.end())
        {
        //    printf("%i found in vector\n",i);
            for (const auto& itm : _map)
                if (itm.second==i)
                {
                    _del=false;
                    break;
                }
            if (_del)
                while(itv!=vect.end())
                {
           //         cout<<"erase from vector\n"<<endl;
                    vect.erase(itv);
                    itv=find(vect.begin(),vect.end(), i);
                }
        }
        else
        {
        //    printf("%i not found in vector\n",i);
            for(auto itm=_map.cbegin(); itm!=_map.cend();)
                    if (itm->second==i)
                        itm=_map.erase(itm);
                    else
                        ++itm;
        }
    }
}

// - - - - - - - - - - - - - - - - - - - -
int main()
{
    unsigned int nContainerSize;
    vector<char> _vector;
    map<unsigned int,char> _map;

    srand(static_cast<unsigned int>(time(nullptr)));
    cout<<"Enter container size ";
    cin>>nContainerSize;

    initContainers(_vector, _map, nContainerSize);
    printContainers(_vector, _map);
    cutFromVector(_vector);
    cutFromMap(_map);
    printContainers(_vector,_map);
    intersectionContainers(_vector,_map);
    printContainers(_vector, _map);
    _vector.clear();
    _map.clear();
    cout<<"Press any key"<<endl;
    return 0;
}
