#include <iostream>

#include <math.h>

int main() {
    int i;
    int j;
    int k;
    int l;
    int m;
    int n;
    int o;
    i = 1;
    j = 0;
    if(i > 0){
        j = 1;
    }else if(i == 1){
        j = 2;
    }else{
        j = 3;
    }
    k = 0;
    if(i < 0){
        k = 1;
    }else if(i == 1){
        k = 2;
    }else{
        k = 3;
    }
    l = 0;
    if(i < 0){
        l = 1;
    }else if(i < 1){
        l = 2;
    }
    m = 0;
    if(i == 1){
        m = 1;
    }
    n = 0;
    if(i < 1){
        n = 1;
    }else if(i > 0){
        n = 2;
    }
    o = 0;
    if(i < 1){
        o = 1;
    }else{
        o = 3;
    }
    return 0;
}
