## max_flow 最大フロー/最小カット
計算量は $O(VF)$. ただし $F$ は最大容量．辺の容量はすべて整数とする．  
`max_flow(int n)`  
`void add(u,v,cost)`  
`int64_t flow(s,t)`