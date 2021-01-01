//
// Created by Jenny Pruitt on 6/26/19.
//
//finds dominant color of the image by taking the center of the image and returns the HSV of just the dominant color
#include "DominantColor.h"

using namespace cv;
using namespace std;



cv::Mat DominantColor::get_dominant_palette(std::vector<cv::Vec3b> colors) {
    const int tile_size = 64;
    cv::Mat ret = cv::Mat(tile_size, tile_size*colors.size(), CV_8UC3, cv::Scalar(0));

    for(int i=0;i<colors.size();i++) {
        cv::Rect rect(i*tile_size, 0, tile_size, tile_size);
        cv::rectangle(ret, rect, cv::Scalar(colors[i][0], colors[i][1], colors[i][2]), FILLED);
    }

    return ret;
}

std::vector<t_color_node*> DominantColor::get_leaves(t_color_node *root) {
    std::vector<t_color_node*> ret;
    std::queue<t_color_node*> queue;
    queue.push(root);

    while(queue.size() > 0) {
        t_color_node *current = queue.front();
        queue.pop();

        if(current->left && current->right) {
            queue.push(current->left);
            queue.push(current->right);
            continue;
        }

        ret.push_back(current);
    }

    return ret;
}

std::vector<cv::Vec3b> DominantColor::get_dominant_colors(t_color_node *root) {
    std::vector<t_color_node*> leaves = get_leaves(root);
    std::vector<cv::Vec3b> ret;

    for(int i=0;i<leaves.size();i++) {
        cv::Mat mean = leaves[i]->mean;
        ret.push_back(cv::Vec3b(mean.at<double>(0)*255.0f,
                                mean.at<double>(1)*255.0f,
                                mean.at<double>(2)*255.0f));
    }

    return ret;
}

int DominantColor::get_next_classid(t_color_node *root) {
    int maxid = 0;
    std::queue<t_color_node*> queue;
    queue.push(root);

    while(queue.size() > 0) {
        t_color_node* current = queue.front();
        queue.pop();

        if(current->classid > maxid)
            maxid = current->classid;

        if(current->left != NULL)
            queue.push(current->left);

        if(current->right)
            queue.push(current->right);
    }

    return maxid + 1;
}

void DominantColor::get_class_mean_cov(cv::Mat img, cv::Mat classes, t_color_node *node) {
    const int width = img.cols;
    const int height = img.rows;
    const uchar classid = node->classid;

    cv::Mat mean = cv::Mat(3, 1, CV_64FC1, cv::Scalar(0));
    cv::Mat cov = cv::Mat(3, 3, CV_64FC1, cv::Scalar(0));

    // We start out with the average color
    double pixcount = 0;
    for(int y=0;y<height;y++) {
        cv::Vec3b* ptr = img.ptr<cv::Vec3b>(y);
        uchar* ptrClass = classes.ptr<uchar>(y);
        for(int x=0;x<width;x++) {
            if(ptrClass[x] != classid)
                continue;

            cv::Vec3b color = ptr[x];
            cv::Mat scaled = cv::Mat(3, 1, CV_64FC1, cv::Scalar(0));
            scaled.at<double>(0) = color[0]/255.0f;
            scaled.at<double>(1) = color[1]/255.0f;
            scaled.at<double>(2) = color[2]/255.0f;

            mean += scaled;
            cov = cov + (scaled * scaled.t());

            pixcount++;
        }
    }

    cov = cov - (mean * mean.t()) / pixcount;
    mean = mean / pixcount;

    // The node mean and covariance
    node->mean = mean.clone();
    node->cov = cov.clone();

    return;
}

void DominantColor::partition_class(cv::Mat img, cv::Mat classes, uchar nextid, t_color_node *node) {
    const int width = img.cols;
    const int height = img.rows;
    const int classid = node->classid;

    const uchar newidleft = nextid;
    const uchar newidright = nextid+1;

    cv::Mat mean = node->mean;
    cv::Mat cov = node->cov;
    cv::Mat eigenvalues, eigenvectors;
    cv::eigen(cov, eigenvalues, eigenvectors);

    cv::Mat eig = eigenvectors.row(0);
    cv::Mat comparison_value = eig * mean;

    node->left = new t_color_node();
    node->right = new t_color_node();

    node->left->classid = newidleft;
    node->right->classid = newidright;

    // We start out with the average color
    for(int y=0;y<height;y++) {
        cv::Vec3b* ptr = img.ptr<cv::Vec3b>(y);
        uchar* ptrClass = classes.ptr<uchar>(y);
        for(int x=0;x<width;x++) {
            if(ptrClass[x] != classid)
                continue;

            cv::Vec3b color = ptr[x];
            cv::Mat scaled = cv::Mat(3, 1,
                                     CV_64FC1,
                                     cv::Scalar(0));

            scaled.at<double>(0) = color[0]/255.0f;
            scaled.at<double>(1) = color[1]/255.0f;
            scaled.at<double>(2) = color[2]/255.0f;

            cv::Mat this_value = eig * scaled;

            if(this_value.at<double>(0, 0) <= comparison_value.at<double>(0, 0)) {
                ptrClass[x] = newidleft;
            } else {
                ptrClass[x] = newidright;
            }
        }
    }
    return;
}

cv::Mat DominantColor::get_quantized_image(cv::Mat classes, t_color_node *root) {
    std::vector<t_color_node*> leaves = get_leaves(root);

    const int height = classes.rows;
    const int width = classes.cols;
    cv::Mat ret(height, width, CV_8UC3, cv::Scalar(0));

    for(int y=0;y<height;y++) {
        uchar *ptrClass = classes.ptr<uchar>(y);
        cv::Vec3b *ptr = ret.ptr<cv::Vec3b>(y);
        for(int x=0;x<width;x++) {
            uchar pixel_class = ptrClass[x];
            for(int i=0;i<leaves.size();i++) {
                if(leaves[i]->classid == pixel_class) {
                    ptr[x] = cv::Vec3b(leaves[i]->mean.at<double>(0)*255,
                                       leaves[i]->mean.at<double>(1)*255,
                                       leaves[i]->mean.at<double>(2)*255);
                }
            }
        }
    }

    return ret;
}

cv::Mat DominantColor::get_viewable_image(cv::Mat classes) {
    const int height = classes.rows;
    const int width = classes.cols;

    const int max_color_count = 12;
    cv::Vec3b *palette = new cv::Vec3b[max_color_count];
    palette[0]  = cv::Vec3b(  0,   0,   0);
    palette[1]  = cv::Vec3b(255,   0,   0);
    palette[2]  = cv::Vec3b(  0, 255,   0);
    palette[3]  = cv::Vec3b(  0,   0, 255);
    palette[4]  = cv::Vec3b(255, 255,   0);
    palette[5]  = cv::Vec3b(  0, 255, 255);
    palette[6]  = cv::Vec3b(255,   0, 255);
    palette[7]  = cv::Vec3b(128, 128, 128);
    palette[8]  = cv::Vec3b(128, 255, 128);
    palette[9]  = cv::Vec3b( 32,  32,  32);
    palette[10] = cv::Vec3b(255, 128, 128);
    palette[11] = cv::Vec3b(128, 128, 255);

    cv::Mat ret = cv::Mat(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
    for(int y=0;y<height;y++) {
        cv::Vec3b *ptr = ret.ptr<cv::Vec3b>(y);
        uchar *ptrClass = classes.ptr<uchar>(y);
        for(int x=0;x<width;x++) {
            int color = ptrClass[x];
            if(color >= max_color_count) {
                printf("You should increase the number of predefined colors!\n");
                continue;
            }
            ptr[x] = palette[color];
        }
    }

    return ret;
}

t_color_node* DominantColor::get_max_eigenvalue_node(t_color_node *current) {
    double max_eigen = -1;
    cv::Mat eigenvalues, eigenvectors;

    std::queue<t_color_node*> queue;
    queue.push(current);

    t_color_node *ret = current;
    if(!current->left && !current->right)
        return current;

    while(queue.size() > 0) {
        t_color_node *node = queue.front();
        queue.pop();

        if(node->left && node->right) {
            queue.push(node->left);
            queue.push(node->right);
            continue;
        }

        cv::eigen(node->cov, eigenvalues, eigenvectors);
        double val = eigenvalues.at<double>(0);
        if(val > max_eigen) {
            max_eigen = val;
            ret = node;
        }
    }

    return ret;
}

std::vector<cv::Vec3b> DominantColor::find_dominant_colors(cv::Mat img, int count) {
    const int width = img.cols;
    const int height = img.rows;

    cv::Mat classes = cv::Mat(height, width, CV_8UC1, cv::Scalar(1));
    t_color_node *root = new t_color_node();

    root->classid = 1;
    root->left = NULL;
    root->right = NULL;

    t_color_node *next = root;
    get_class_mean_cov(img, classes, root);
    for(int i=0;i<count-1;i++) {
        next = get_max_eigenvalue_node(root);
        partition_class(img, classes, get_next_classid(root), next);
        get_class_mean_cov(img, classes, next->left);
        get_class_mean_cov(img, classes, next->right);
    }

    std::vector<cv::Vec3b> colors = get_dominant_colors(root);

    cv::Mat quantized = get_quantized_image(classes, root);
    cv::Mat viewable = get_viewable_image(classes);
    cv::Mat dom = get_dominant_palette(colors);

    cv::imshow("Viewable", viewable);
    cv::imshow("Quantized", quantized);
    cv::imshow("Palette", dom);

    waitKey();
    return colors;
}

void DominantColor::calculateHistogram(cv::Mat frame){
    Mat hsv;
    cvtColor(frame, hsv, COLOR_BGR2HSV);

    // Quantize the hue to 30 levels
    // and the saturation to 32 levels
    int hbins = 30, sbins = 32;
    int histSize[] = {hbins, sbins};
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = { 0, 180 };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };
    MatND hist;
    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = {0, 1};

    calcHist( &hsv, 1, channels, Mat(), // do not use mask
              hist, 2, histSize, ranges,
              true, // the histogram is uniform
              false );
    double maxVal=0;
    minMaxLoc(hist, 0, &maxVal, 0, 0);

    int scale = 10;
    Mat histImg = Mat::zeros(sbins*scale, hbins*10, CV_8UC3);

    int maxIntensity = -100;
    for( int h = 0; h < hbins; h++ ) {
        for( int s = 0; s < sbins; s++ )
        {
            float binVal = hist.at<float>(h, s);
            int intensity = cvRound(binVal*255/maxVal);
            rectangle( histImg, Point(h*scale, s*scale),
                       Point( (h+1)*scale - 1, (s+1)*scale - 1),
                       Scalar::all(intensity),
                       FILLED );
            if(intensity > maxIntensity)
                maxIntensity = intensity;
        }
    }
    cout<<maxVal<<endl;



    namedWindow( "H-S Histogram", 1 );
    imshow( "H-S Histogram", histImg );
    cv::waitKey();
}

