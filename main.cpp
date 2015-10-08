/**
* eye-tracking.cpp:
* Eye detection and tracking with OpenCV
*
* This program tries to detect and tracking the user's eye with webcam.
* At startup, the program performs face detection followed by eye detection
* using OpenCV's built-in Haar cascade classifier. If the user's eye detected
* successfully, an eye template is extracted. This template will be used in
* the subsequent template matching for tracking the eye.
*/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <vector>

cv::CascadeClassifier face_cascade;
cv::CascadeClassifier eye_cascade;
const int WIDTH = 640, HEIGHT = 480;
//const cv::Rect run{};
/**
* Function to detect human face and the eyes from an image.
*
* @param im The source image
* @param tpl Will be filled with the eye template, if detection success.
* @param rect Will be filled with the bounding box of the eye
* @return zero=failed, nonzero=success
*/
auto detectEye(cv::Mat const& im, cv::Mat & tpl, cv::Rect & rect)
{
    std::vector<cv::Rect> faces, eyes;
    face_cascade.detectMultiScale(im, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30,30));
    for (int i = 0; i < faces.size(); i++)
    {
        cv::Mat face = im(faces[i]);
        eye_cascade.detectMultiScale(face, eyes, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(20,20));
        if (eyes.size())
        {
            rect = eyes[0] + cv::Point(faces[i].x, faces[i].y);
            tpl = im(rect);
        }
    }
    return eyes.size();
}

/**
* Perform template matching to search the user's eye in the given image.
*
* @param im The source image
* @param tpl The eye template
* @param rect The eye bounding box, will be updated with the new location of the eye
*/
auto trackEye(cv::Mat const& im, cv::Mat& tpl, cv::Rect& rect)
{
    cv::Size size(rect.width * 2, rect.height * 2);
    cv::Rect window(rect + size - cv::Point(size.width/2, size.height/2));
    window &= cv::Rect(0, 0, im.cols, im.rows);
    cv::Mat dst(window.width - tpl.rows + 1, window.height - tpl.cols + 1, CV_32FC1);
    cv::matchTemplate(im(window), tpl, dst, CV_TM_SQDIFF_NORMED);
    double minval, maxval;
    cv::Point minloc, maxloc;
    cv::minMaxLoc(dst, &minval, &maxval, &minloc, &maxloc);
    if (minval <= 0.2)
    {
        rect.x = window.x + minloc.x;
        rect.y = window.y + minloc.y;
    }
    else
        rect.x = rect.y = rect.width = rect.height = 0;
}

auto center_of_rect(cv::Rect const& rect)
{
    return cv::Point{ rect.x + rect.width / 2, rect.y + rect.height / 2 };
}

int main(int argc, char** argv)
{
    // Load the cascade classifiers
    // Make sure you point the XML files to the right path, or
    // just copy the files from [OPENCV_DIR]/data/haarcascades directory
    face_cascade.load("haarcascade_frontalface_alt2.xml");
    eye_cascade.load("haarcascade_eye.xml");

    std::cout << "==================\n";

    // Open webcam
    cv::VideoCapture cap(0);
    // Check if everything is ok
    if (face_cascade.empty() || eye_cascade.empty() || !cap.isOpened())
    {
        std::cout << "bad\n";
        return 1;
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
    cv::Mat frame, eye_tpl;
    cv::Rect eye_bb;
    while (cv::waitKey(15) != 'q' && cv::waitKey(15) != 'Q')
    {
        cap >> frame;
        if (frame.empty()) break;
        // Flip the frame horizontally, Windows users might need this
        cv::flip(frame, frame, 1);
        // Convert to greyscale and
        // adjust the image contrast using histogram equalization
        cv::Mat gray;
        cv::cvtColor(frame, gray, CV_BGR2GRAY);
        if (eye_bb.width == 0 && eye_bb.height == 0)
        {
            // Detection stage
            // Try to detect the face and the eye of the user
            detectEye(gray, eye_tpl, eye_bb);
        }
        else
        {
            // Tracking stage with template matching
            trackEye(gray, eye_tpl, eye_bb);
            // Draw bounding rectangle for the eye
            cv::rectangle(frame, eye_bb, CV_RGB(0,255,0));
        }

        {//drawing grids

            struct Line { cv::Point from, to; };
            using Lines = std::vector<Line>;
            Lines lines{
                    { { 213, 0 }, { 213, 480 } },
                    { { 427, 0 }, { 427, 480 } },
                    { { 0, 160 }, { 640, 160 } },
                    { { 0, 320 }, { 640, 320 } }
            };

            for (auto const& l : lines)
                cv::line(frame,l.from, l.to, CV_RGB(0,255,0), 1, 1);
        }

        {
            std::vector<cv::Rect> direction_boxes{
                    cv::Rect{cv::Point{213,   0}, cv::Point{427, 160}}, //F
                    cv::Rect{cv::Point{  0, 160}, cv::Point{213, 320}}, //L
                    cv::Rect{cv::Point{427, 160}, cv::Point{640, 320}}  //R
            };

            auto draw_direction = [&](std::string const &direction) {
                cv::putText(frame, direction, cv::Point{280, 435}, cv::FONT_HERSHEY_DUPLEX, 3, CV_RGB(70, 130, 180),  5);
                cv::putText(frame, direction, cv::Point{280, 435}, cv::FONT_HERSHEY_DUPLEX, 3, CV_RGB(102, 105, 170), 4);
            };

            for(int box = 0; box != 3; ++box)
            {
                if (box == 0)
                    if (direction_boxes[0].contains(center_of_rect(eye_bb)))
                    {
                        draw_direction("F");
                        break;
                    }
                if (box == 1)
                    if (direction_boxes[1].contains(center_of_rect(eye_bb)))
                    {
                        draw_direction("L");
                        break;
                    }
                if (box == 2)
                    if (direction_boxes[2].contains(center_of_rect(eye_bb)))
                    {
                        draw_direction("R");
                        break;
                    }
            }
            std::cout << center_of_rect(eye_bb).x << std::endl;
        }

        cv::imshow("video", frame);
    }
    return 0;
}
