#ifndef polynomial_header
#define polynomial_header

class Polynomial
{
private:
	long *a;
	long deg;

 	long getDegree()
  	{
    	for (long x=deg; x>=0; x--)
	    	if (a[x]!=0) return x;

    	return 0;
  	}

  	void normalize()
  	{
	    long deg=getDegree();
    
		if (this->deg!=deg)
		{
	    	long *na=new long[deg+1];

		    for (long x=0; x<=deg; x++)
	      		na[x]=a[x];
	
	    	delete []a;
	    	
	    	a=na;
	    	this->deg=deg;
		}
  	}

  	void moddiv(Polynomial* p)
  	{
    	long deg=getDegree();
    	long pdeg=p->deg;

	    if (deg>=pdeg)
	    {
			Polynomial b(p);

    		while(deg>=pdeg)
    		{
      			Polynomial c(deg);

      			const long ab=a[deg]/b.a[pdeg];
      			const long deg_pdeg=deg-pdeg;

      			for (long i=deg; i>=deg_pdeg; i--)
        			c.a[i]=a[i] - b.a[i-deg_pdeg] * ab;

      			for (long i=deg; i>=deg_pdeg; i--)
        			a[i]=c.a[i];

      			deg--;
    		}

			this->deg=deg;
    		normalize();
	    }
	}

public:
	static Polynomial* createX(long n)
	{
		Polynomial* p=new Polynomial(n);
		p->a[n]=1;
		
		return p;
	}

	static Polynomial* createX()
	{
		Polynomial* p=new Polynomial(1);
		p->a[1]=1;
		
		return p;
	}

	static Polynomial* createXa(long n, long aa)
	{
		Polynomial* p=createX(n);
		p->a[0]=aa;
		
		return p;
	}

	//-------------------------------------------------------------------------
	Polynomial()
	{
		a=new long[1];
		a[0]=0;
		deg=0;
	}

	Polynomial(long degree)
	{
		a=new long[degree+1];
		deg=degree;
		
		for (long x=0; x<=degree; x++)
			a[x]=0;
	}

	Polynomial(Polynomial *p)
	{
		a=0;
		valueOf(p);
	}

	~Polynomial()
	{
		delete[] a;
	}

	void valueOf(Polynomial* p)
	{
		deg=p->deg;
		
		if (a) delete[] a;
		a=new long[deg+1];
		
		for (long x=0; x<=deg; x++)
			a[x]=p->a[x];
	}

	long compareTo(Polynomial* p)
	{
		if (deg==p->deg)
		{
			for (long x=deg; x>=0; x--)
				if (a[x]<p->a[x])
					return 1;
				else if (a[x]>p->a[x])
					return -1;
		}
		else
			return p->deg-deg;
	
		return 0;
	}

	void mul(long a)
	{
		for (long x=0; x<=deg; x++)
			this->a[x]*=a;
	}

	void mul(Polynomial* p)
	{
		long *na=new long[deg+p->deg+1];
		
		for (long x=0; x<=deg+p->deg; x++)
			na[x]=0;

		for (long x=0; x<=deg; x++)
		for (long y=0; y<=p->deg; y++)
			na[x+y]+=a[x]*p->a[y];
	
		delete []a;
		a=na;
        deg+=p->deg-1;
	}

	void mulmod(Polynomial* p, long a)
	{
		long *na=new long[deg+p->deg+1];
		
		for (long x=0; x<=deg+p->deg; x++)
			na[x]=0;

		for (long x=0; x<=deg; x++)
		for (long y=0; y<=p->deg; y++)
                    na[x+y]+=((this->a[x]%a)*(p->a[y]%a))%a;

		delete[] this->a;
		
		this->a=na;
		deg=deg+p->deg;
		mod(a);
	}

	void pow(long n)
	{
		Polynomial p(this);

		for (long x=1; x<n; x++)
			mul(&p);
	}

	void powmod(long n, long a)
	{
		Polynomial p(this);

		for (long x=1; x<n; x++)
			mulmod(&p, a);
	}

	void mod(long a)
	{
		for (long x=0; x<=deg; x++)
			this->a[x]%=a;
	}

	void mod(Polynomial* p)
	{
		moddiv(p);
	}
};

#endif

