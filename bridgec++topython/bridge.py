# python objects like list or integers looks very differnt in the memory than c++ objects does
import ctypes  #we load the ctypes library to translate or convert the int , float etc
import os     #we load the os library to talk with the os 

lib_path = os.path.join(os.getcwd(), "closestpairpointlib.so")

#os.pat.json  to find the correct folder path windows uses \ but the linux uses / this function decide its own self what to add slash.
#os.getcwd() this function tell us where are we staning in which directory 

try:
    closest_lib = ctypes.CDLL(lib_path)
    print(f"Successfully loaded library from: {lib_path}")
except OSError:
    print(f"Error: Could not find 'libclosest.so'. Make sure it is in this folder!")
    exit()

# ctypes.CDLL(lib_path) this command will actually load the .so file in to memory and the 
# closest_lib  thrugh this variable we will call c++ functions

closest_lib.find_closest_pair.argtypes = [
    ctypes.POINTER(ctypes.c_double), # 1. X coordinates array
    ctypes.POINTER(ctypes.c_double), # 2. Y coordinates array
    ctypes.c_int                     # 3. Points count(n)
]
closest_lib.find_closest_pair.restype = ctypes.c_double

#closest_lib.find_closest_pair.argtypes -- defining that the c++ function will accept  3 arguments 
#ctypes.POINTER  telling python that im giving you a list but forward after making it pointers

#ctypes.c_double converting float to double as the float in python and the double in the c++ is not the same so convert it.
#restype when the functions ends it will know that we are giving double (distance )

def run_cpp_algorithm(points):   # defining a function with name and pass a parameter
    n = len(points)       # len will count the items inside the list !
    
    # Python List: [(1,2), (3,4)] -> X: [1, 3], Y: [2, 4] list of tuples 
    x_list = [p[0] for p in points] # this is List Comprehension here the from the list take the point and t=its first part which is p[0] or x cordinate and put it to the new list 
    y_list = [p[1] for p in points] # same for y cordinate 
    ArrayType = ctypes.c_double * n   # defining a c++ style array  whose size is exactly n size
    
    x_c_array = ArrayType(*x_list) # Unpack the Python list values and fill them into the C++ array
    y_c_array = ArrayType(*y_list) # Unpack the Python list values and fill them into the C++ array
    
    return closest_lib.find_closest_pair(x_c_array, y_c_array, n)

# x_c_array: X coordinates list 
#y_c_array: Y coordinates list

# this part is for me to check  if the file is being run directly  for only my testig purpose
# If Member 3 imports this file this block will not run.
if __name__ == "__main__":
# define some dummy data to test the logic
    test_points = [(2, 3), (12, 30), (40, 50), (5, 1), (12, 10), (3, 4)]
# is it woring or not 
    print(f"Result: {run_cpp_algorithm(test_points)}")


