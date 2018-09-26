#include <iostream>
#include <cstdlib>
#include <hello_libs.hpp>

int main()
{
    int n = 2, m = 2;
    std::cout << "Find largest element in matrix." << std::endl;
    std::cout << std::endl;   
    //std::cout << "Enteratrix size NxM." << std::endl; 
    //std::cout << "N: "; std::cin >> n;
    //std::cout << std::endl;
    //std::cout << "M: "; std::cin >> m;
    //std::cout << std::endl;
    //std::cout << "Enter matrix elements: " << std::endl; 
    std::cout << "Matrix " << n  << "x" << m << std::endl; 
    
    int *a = new int[n*m];
    a[0] = 2;
    a[1] = 12;
    a[2] = 44;
    a[3] = 15;

    /*for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            std::cin >> a[i*m + j];
        }
        std::cout << std::endl;
    }*/


    bool is_good = matrix_largest_elem(reinterpret_cast<int*>(a), n , m);

    int result = is_good ? EXIT_SUCCESS : EXIT_FAILURE;
    return result;
}	
