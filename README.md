# MOSP Delta Evaluation
Fast evaluation procedures for the Minimization of Open Stacks and Gate Matrix Layout Problems

Please find the method description in the technical report below 

J. R. Lima, V. G. M. Santos, and M. A. M. Carvalho, A Δ-evaluation function for column permutation problems. 2024. [Online]. Available: https://arxiv.org/abs/2409.04926.

How to use: Import the 'mosp.cpp' file into your project's includes

Requirements:
	1st - Vector with a feasible solution (valid permutation) for the problem;
	2nd - Vector with the total demand for each product (i.e., how many times a product is demanded by customers in the problem);
	3rd - Vector with lists containing the products ordered by each customer in the solution.
	
Implementation:
	1st - Initialize the one structures that will be used in the delta evaluation using the 'initializeOnesStructure' method;
	2nd - Use the 'calculateDelta' method to perform the delta evaluation.
	
Note: Other instructions are in the methods documentation.
