#include <iostream>

bool matrix_largest_elem(int *A, int n, int m) {
    for(int i = 0; i < n; i++)
    {
       for(int j = 0; j < m; j++) 
       {
           if(A[0] < A[i*m + j])
           A[0] = A[i*m + j];
       }
    }
    std::cout << "Largest element = " << A[0] << std::endl;

    if(n < 0 || m < 0)
	return false;
    
    return true;
}
