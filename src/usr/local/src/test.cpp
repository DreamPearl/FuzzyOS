// Simple C++ program
#include <iostream.h>
#include <string.h>
#include <vector.h>

using namespace std;
int main(int argc, char *argv[]) {

    std::vector<std::vector<int> > v;
    // cout<<"vsize: "<<(int)v.size()<<endl;
    // v.resize(5, std::vector<int>());
    // cout<<"vsize: "<<(int)v.size()<<endl;
    // for(int i=0;i<v.size();i++) v[i].push_back(i);
    for(int i=0;i<3;i++) {
        auto a2 = std::vector<int>(1,i);
        std::cout<<"let's push again, a2 = " << (int)&a2[0] <<"\n";
        v.push_back(a2);
        for(int i=0;i<v.size();i++) {
            cout<<v[i][0];
        }
        cout<<endl;
        for(int i=0;i<v.size();i++) {
            cout<<(int)&v[i]<<" ";
        }
        cout<<endl;
        for(int i=0;i<v.size();i++) {
            cout<<(int)&v[i][0]<<" ";
        }
        cout<<endl;
    }
    cout<<"vsize: "<<(int)v.size()<<endl;
    // for(int i=v.size();i>1;i--) {
    //     v.resize(i);
    //     cout<<"vsize["<<i<<"]: "<<(int)v[i].size()<<endl;
    //     for(int i=0;i<v.size();i++) {
    //         cout<<v[i][0];
    //     }
    //     cout<<endl;
    // }




    return 0;
}