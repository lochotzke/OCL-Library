__kernel void vectoradd(const int N,  __global float *a, __global float *b, __global float *c){

  // find coordinate of thread in global axis-0 
  int n = get_global_id(0);

  if(n<N)
    c[n] = a[n] + b[n];
}
