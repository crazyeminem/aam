/**
This file is part of Active Appearance Models (AAM).

Copyright Christoph Heindl 2015
Copyright Sebastian Zambal 2015

AAM is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

AAM is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with AAM.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <aam/aam.h>
#include <aam/trainer.h>
#include <aam/io.h>
#include <aam/show.h>
#include <aam/barycentrics.h>
#include <aam/rasterization.h>
#include <aam/map.h>

#include <opencv2/highgui/highgui.hpp>
#include <iostream>


/**

Main entry point.

*/
int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " directory" << std::endl;
        return 0;
    }

    aam::TrainingSet trainingSet;
    aam::loadAsfTrainingSet(argv[1], trainingSet);

    aam::Trainer::createTriangulation(trainingSet);

    aam::Trainer trainer(trainingSet);
    aam::ActiveAppearanceModel model;
    trainer.train(model);

    //model.save("test.model");
    //model.load("test.model");

    cv::Mat img(640, 480, CV_8U);
    aam::RowVectorX shapeParams = model.shapeModeWeights * 0;
    aam::RowVectorX appearanceParams = model.appearanceModeWeights * 0;

    int key = 0;
    int counter = 0;
    do {
        img = cv::Scalar(0);

        shapeParams(shapeParams.cols()-1) = (aam::Scalar)(std::sin((float)counter / 180 * 20));
        appearanceParams(appearanceParams.cols()-1) = (aam::Scalar)(std::sin((float)counter / 180 * 20));
        model.renderAppearanceInstanceToImage(img, aam::MatrixX(0, 0), shapeParams, appearanceParams);

        cv::imshow("AAM instance", img);
        key = cv::waitKey(50);

        std::cout << "counter = " << counter << std::endl;

        counter++;
    } while(key != 27);

    return 0;
}




