#include <iostream>
#include <cstdio>
#include <windows.h>
using namespace std;

char val[15];

void cal()
{
	int i,j,k;
	for(i=1;i<=100;i++)
	{
		for(j=1;j<=100;j++)
		{
			for(k=1;k<=100;k++)
			{
			}
		}
	}
}

int main()
{
	scanf("%s",val);
	int len = strlen(val);

	if(len==8)
	{
		cal();
		if(val[0] == 'a')
		{
			cal();
			if(val[1] == 's')
			{
				cal();
				if(val[2] == 'd')
				{
					cal();
					if(val[3] == 'f')
					{
						cal();
						if(val[4] == 'g')
						{
							cal();
							if(val[5] == 'h')
							{
								cal();
								if(val[6] == 'j')
								{
									cal();
									if(val[7] == 'k')
									{
										cal();
										int a=123;
										int b=23;
										cout<<"You got the right answer"<<endl;
										return 0;
									}
									else
									{
										cout<<"You got the wrong answer"<<endl;
										return 0;
									}
								}
								else
								{
									cout<<"You got the wrong answer"<<endl;
									return 0;
								}
							}
							else
							{
								cout<<"You got the wrong answer"<<endl;
								return 0;
							}
						}
						else
						{
							cout<<"You got the wrong answer"<<endl;
							return 0;
						}
					}
					else
					{
						cout<<"You got the wrong answer"<<endl;
						return 0;
					}
				}
				else
				{
					cout<<"You got the wrong answer"<<endl;
					return 0;
				}
			}
			else
			{
				cout<<"You got the wrong answer"<<endl;
				return 0;
			}
		}
		else
		{
			cout<<"You got the wrong answer"<<endl;
			return 0;
		}
	}
	else
	{
		cout<<"You got the wrong answer"<<endl;
		return 0;
	}
}