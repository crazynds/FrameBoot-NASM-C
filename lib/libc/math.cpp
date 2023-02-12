
/*estava com um bug mas foi consertado*/
int pow(int ex,int el){
    if(el==0)return 1;
    int aux=ex,x=1;
    while(x<el){
        aux*=ex;
        x++;
    }
    return aux;
}
