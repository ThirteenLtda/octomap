#include <memory>
#include <octomap/octomap.h>
#include <string>
#include <iostream>
#include <math.h>
#include "testing.h"

int main(int argc, char **argv) 
{
    const std::vector<octomap::point3d> ground_truth = 
    {
        octomap::point3d(-0.25, 0.05, 0.05), octomap::point3d(-0.25, 0.15, 0.05), 
        octomap::point3d(-0.15, 0.05, 0.05), octomap::point3d(-0.05, 0.05, 0.05),
        octomap::point3d(-0.15, 0.15, 0.05), octomap::point3d(-0.05, 0.15, 0.05), 
        octomap::point3d(-0.25, 0.25, 0.05), octomap::point3d(-0.15, 0.25, 0.05), 
        octomap::point3d(-0.05, 0.25, 0.05), octomap::point3d(0.05, 0.05, 0.05), 
        octomap::point3d(0.15, 0.05, 0.05),  octomap::point3d(0.05, 0.15, 0.05), 
        octomap::point3d(0.15, 0.15, 0.05),  octomap::point3d(0.25, 0.05, 0.05), 
        octomap::point3d(0.25, 0.15, 0.05),  octomap::point3d(0.05, 0.25, 0.05), 
        octomap::point3d(0.15, 0.25, 0.05),  octomap::point3d(0.25, 0.25, 0.05)
    };

    const std::vector<float> intensities =
    {
        1.15, 1.16, 1.12, 1.09, 1.13, 1.1, 1.17, 1.14, 1.11, 1.0, 1.03, 1.01, 1.04, 
        1.06, 1.07, 1.02, 1.05, 1.08
    };

    std::unique_ptr<octomap::OcTree> tree;
    tree = std::make_unique<octomap::OcTree>(0.1);
    tree->setOccupancyThres(0.5);
    for (size_t i = 0; i <  ground_truth.size(); ++i)
        tree->setNodeValue(ground_truth[i], intensities[i]);

    auto cells = tree->getOccupiedCells();
    for (size_t i = 0 ; i < cells.size(); ++i)
    {
        EXPECT_NEAR(cells[i].first.x(), ground_truth[i].x(), 1e-6);
        EXPECT_NEAR(cells[i].first.y(), ground_truth[i].y(), 1e-6);
        EXPECT_NEAR(cells[i].first.z(), ground_truth[i].z(), 1e-6);
        EXPECT_NEAR(cells[i].second, intensities[i], 1e-6);
    }

    cells = tree->getOccupiedCells(0.1);
    for (size_t i = 0 ; i < cells.size(); ++i)
    {
        EXPECT_NEAR(cells[i].first.x(), ground_truth[i].x(), 1e-6);
        EXPECT_NEAR(cells[i].first.y(), ground_truth[i].y(), 1e-6);
        EXPECT_NEAR(cells[i].first.z(), ground_truth[i].z(), 1e-6);
        EXPECT_NEAR(cells[i].second, intensities[i], 1e-6);
    }

    cells = tree->getOccupiedCells(0.05);
    for (size_t i = 0 ; i < cells.size(); ++i)
    {
        EXPECT_NEAR(cells[i].first.x(), ground_truth[i].x(), 1e-6);
        EXPECT_NEAR(cells[i].first.y(), ground_truth[i].y(), 1e-6);
        EXPECT_NEAR(cells[i].first.z(), ground_truth[i].z(), 1e-6);
        EXPECT_NEAR(cells[i].second, intensities[i], 1e-6);
    }

    const std::vector<octomap::point3d> ground_truth_new_resolution = 
    {
        octomap::point3d(-0.25, 0.25, 0.25), octomap::point3d(0.25, 0.25, 0.25)
    };

    const std::vector<float> ground_truth_new_intensities = {1.17, 1.08};


    cells = tree->getOccupiedCells(0.5);
    for (size_t i = 0 ; i < cells.size(); ++i)
    {
        EXPECT_NEAR(cells[i].first.x(), ground_truth_new_resolution[i].x(), 1e-6);
        EXPECT_NEAR(cells[i].first.y(), ground_truth_new_resolution[i].y(), 1e-6);
        EXPECT_NEAR(cells[i].first.z(), ground_truth_new_resolution[i].z(), 1e-6);
        EXPECT_NEAR(cells[i].second, ground_truth_new_intensities[i], 1e-6);
    }
}