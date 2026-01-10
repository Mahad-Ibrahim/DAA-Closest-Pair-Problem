/*
 *  File: closest_pair.cpp
 *  Description: Contains source code for the divide and conquor
 *   and brute force implementation of the closest pair problem.
 *  Optimization: Uses Manhattan distance instead of Euclidean distance.
 */

#include <vector>
#include <algorithm>
#include <cmath>
#include <cfloat>

using namespace std;   

    /* Point structure to represent a point in 2D space */
struct Point {
    double x, y;
};

    /* 
     *  Function to compare points based on x and y 
     *  coordinates. It returns the manhattan distance
     *  between two points.
     */

double manhattan_dist(const Point a, const Point b){
    return abs(a.x - b.x) + abs(a.y - b.y);
}

    /*
     *  Brute force method to find the closest pair of points
     *  in a small set of points.
     */

double brute_force(const vector<Point>& points) {
    double min_dist = DBL_MAX;
    unsigned int size = points.size();
    for (int i = 0; i < size; i++){
        for (int j = i + 1; j < size; j++){
            double dist = manhattan_dist(points[i], points[j]);
            if (min_dist > dist){
                min_dist = dist;
            }
        }
    }
    return min_dist;
}



    /*
     *  Function that will divide the points into two halves
     *  based on their x-coordinates.
     */

vector<vector<Point>> divide_points(const vector<Point>& points){
    vector<vector<Point>> halves;
    unsigned int size = points.size();
    unsigned int mid = size / 2;

    vector<Point> left_half(points.begin(), points.begin() + mid);
    vector<Point> right_half(points.begin() + mid, points.end());

    halves.push_back(left_half);
    halves.push_back(right_half);
    return halves;
}

    /*
     *  Function to find the minimum distance in the strip
     *  of points within a certain distance from the midline.
     */

double strip_closest(vector<Point>& left, vector<Point>& right, double d) {

    double midPointX = (left.back().x + right.front().x) / 2;
    double left_min_dist = midPointX - d;
    double right_min_dist = midPointX + d;
    vector<Point> strip_points;

    for (int i = 0; i < right.size(); i++){
        if (right[i].x <= right_min_dist)
            strip_points.push_back(right[i]);
        if (right[i].x > right_min_dist)
            break;
    }

    for (int i = left.size()-1; i >= 0; i--){
        if (left[i].x >= left_min_dist)
            strip_points.push_back(left[i]);
        if (left[i].x < left_min_dist)
            break;
    }

    sort(strip_points.begin(), strip_points.end(), [](const Point& a, const Point& b) {
        return a.y < b.y;
    });

    for (int i = 0; i < strip_points.size(); i++){
        for (int j = i + 1; j < strip_points.size() && abs(strip_points[i].y - strip_points[j].y) < d; j++){
            double dist = manhattan_dist(strip_points[i], strip_points[j]);
            if (dist < d)
                d = dist;
        }
    }

    return d;
}

    /*
     *  Function that will divide and conquer to 
     *  find the closest pair of points in the 
     *  2 halves of the set of points.
     */

double closest_pair_dc(const vector<Point>& points) {
    unsigned int size = points.size();

    if (size <= 3)
        return brute_force(points);

    vector<vector<Point>> halves = divide_points(points);
    double dl = closest_pair_dc(halves[0]);
    double dr = closest_pair_dc(halves[1]);
    double d = min(dl, dr);
    
    return strip_closest(halves[0], halves[1], d);
}



extern "C"{
    /* 
     *  This function is the entry point for Python.
     *  It performs the initial Sort-by-X once, then calls the recursive solver.
     */
    double find_closest_pair(double* x_coords, double* y_coords, int n) {
        vector<Point> points(n);
        for (int i = 0; i < n; i++) {
            points[i] = {x_coords[i], y_coords[i]};
        }
        
        /*
         * Inbuilt sorting function in C++, extremely fast.
         * It uses IntroSort (a hybrid sorting algorithm).
         * Time Complexity: O(n log n)
         * 
         * We use it because the divide and conquer assumes 
         * the points are sorted by x-coordinates.
         *
         * The sort function uses a lambda function to compare
         * the x-coordinates of the points.
         * The syntax of the sort function is as follows:
         * sort(start_iterator, end_iterator, comparison_function);
         * The start_iterator and end_iterator define the range
         * of elements to be sorted, it is basically the memory
         * address of the first and last element of the vector.
         * The comparison_function is a function that takes two
         * elements and returns true if the first element is
         * less than the second element, and false otherwise.
         */
        sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
            return a.x < b.x;
        });
        
        return closest_pair_dc(points);
    }

}