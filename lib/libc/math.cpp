
double pow(double x, double y)
{
    if(y==0)return 1;
    double result = x;

    for(double d = 0.0f; d < y; d++)
    {
        result *= x;
    }

    return result;
}

int abs(int i)
{
    return i < 0 ? -i : i;
}

int sqrt(int x)
{
    int quotient = 0;
    int i = 0;

    bool answer_found = false;
    while(answer_found == false)
    {
        if(i * i == x)
        {
          quotient = i;
          answer_found = true;
        }

        i++;
    }

    return quotient;
}
