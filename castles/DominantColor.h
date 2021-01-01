//
// Created by Jenny Pruitt on 6/26/19.
//

#ifndef CPP_COURT_LINE_DOMINANTCOLOR_H
#define CPP_COURT_LINE_DOMINANTCOLOR_H
#include "opencv2/opencv.hpp"

typedef struct t_color_node {
    cv::Mat       mean;       // The mean of this node
    cv::Mat       cov;
    uchar         classid;    // The class ID

    t_color_node  *left;
    t_color_node  *right;
} t_color_node;

class DominantColor {
public:
    std::vector<cv::Vec3b> find_dominant_colors(cv::Mat img, int count);

    void calculateHistogram(cv::Mat frame);

private:
    t_color_node *get_max_eigenvalue_node(t_color_node *current);

    cv::Mat get_viewable_image(cv::Mat classes);

    cv::Mat get_quantized_image(cv::Mat classes, t_color_node *root);

    void partition_class(cv::Mat img, cv::Mat classes, uchar nextid, t_color_node *node);

    void get_class_mean_cov(cv::Mat img, cv::Mat classes, t_color_node *node);

    int get_next_classid(t_color_node *root);

    std::vector<cv::Vec3b> get_dominant_colors(t_color_node *root);

    std::vector<t_color_node*> get_leaves(t_color_node *root);

    cv::Mat get_dominant_palette(std::vector<cv::Vec3b> colors);
};


#endif //CPP_COURT_LINE_DOMINANTCOLOR_H
