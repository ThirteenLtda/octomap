/*
 * This file is part of OctoMap - An Efficient Probabilistic 3D Mapping
 * Framework Based on Octrees
 * http://octomap.github.io
 *
 * Copyright (c) 2009-2014, K.M. Wurm and A. Hornung, University of Freiburg
 * All rights reserved. License for the viewer octovis: GNU GPL v2
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

#include <octovis/TrajectoryDrawer.h>

namespace octomap {

  TrajectoryDrawer::TrajectoryDrawer()
  : ScanGraphDrawer(), m_trajectoryVertexArray(NULL), m_trajectoryColorArray(NULL), m_trajectorySize(0)
  {

  }

  TrajectoryDrawer::TrajectoryDrawer(const octomap::ScanGraph& graph)
  : ScanGraphDrawer(), m_trajectoryVertexArray(NULL), m_trajectoryColorArray(NULL), m_trajectorySize(0)
  {
    this->setScanGraph(graph);
  }

  TrajectoryDrawer::TrajectoryDrawer(const MapCollection<MapNode<OcTree>>& collection)
  : ScanGraphDrawer(), m_trajectoryVertexArray(NULL), m_trajectoryColorArray(NULL), m_trajectorySize(0)
  {
    this->setCollection(collection);
  }

  TrajectoryDrawer::~TrajectoryDrawer() {
    clear();

  }

  void TrajectoryDrawer::draw() const{
    if (m_trajectorySize == 0)
      return;

    // draw lines:
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glLineWidth(3.0f);
    glVertexPointer(3, GL_FLOAT, 0, m_trajectoryVertexArray);
    glColorPointer(4, GL_FLOAT, 0, m_trajectoryColorArray);
    glDrawArrays(GL_LINE_STRIP, 0, m_trajectorySize);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    // draw nodes:
    GLUquadricObj* quadric=gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    for (uint i = 0; i < m_trajectorySize; ++i){
      glPushMatrix();
      glTranslatef(m_trajectoryVertexArray[3*i], m_trajectoryVertexArray[3*i +1], m_trajectoryVertexArray[3*i +2]);
      glColor4f(m_trajectoryColorArray[4*i],m_trajectoryColorArray[4*i+1],m_trajectoryColorArray[4*i+2],m_trajectoryColorArray[4*i+3]);
      gluSphere(quadric, 0.05, 32, 32);
      glPopMatrix();
    }

    gluDeleteQuadric(quadric);
  }

  void TrajectoryDrawer::clear(){

    if (m_trajectorySize != 0) {
      delete[] m_trajectoryVertexArray;
      delete[] m_trajectoryColorArray;
      m_trajectorySize = 0;
    }
  }

  void TrajectoryDrawer::setScanGraph(const octomap::ScanGraph& graph){

    clear();

    m_trajectorySize = graph.size();
    m_trajectoryVertexArray = new GLfloat[m_trajectorySize * 3];
    m_trajectoryColorArray = new GLfloat[m_trajectorySize * 4];

    uint i = 0;
    for (octomap::ScanGraph::const_iterator it = graph.begin(); it != graph.end(); it++) {
      m_trajectoryVertexArray[i] = (*it)->pose.trans().x();
      m_trajectoryVertexArray[i+1] = (*it)->pose.trans().y();
      m_trajectoryVertexArray[i+2] = (*it)->pose.trans().z();
      i+=3;
    }

    for (unsigned int j=0; j < m_trajectorySize*4; j+=4) {
      m_trajectoryColorArray[j]   = 0.; // r
      m_trajectoryColorArray[j+1] = 0.; // g
      m_trajectoryColorArray[j+2] = 1.; // b
      m_trajectoryColorArray[j+3] = 1.; // alpha
    }
  }

 void TrajectoryDrawer::setCollection(const MapCollection<MapNode<OcTree>>& collection)
 {

    clear();

    m_trajectorySize = collection.size();
    m_trajectoryVertexArray = new GLfloat[m_trajectorySize * 3];
    m_trajectoryColorArray = new GLfloat[m_trajectorySize * 4];

    uint i = 0;
    for (size_t index = 0; index < collection.size(); ++index) 
    {
        MapNode<OcTree>* node;
        collection.loadNode(index, node);
        m_trajectoryVertexArray[i] = node->getOrigin().trans().x();
        m_trajectoryVertexArray[i+1] = node->getOrigin().trans().y();
        m_trajectoryVertexArray[i+2] = node->getOrigin().trans().z();
        i+=3;
    }

    static const std::array<double, 3> colorA = {0, 0, 1.0};
    static const std::array<double, 3> colorB = {1.0, 0, 0};

    m_trajectoryColorArray[0] = colorA[0]; // r
    m_trajectoryColorArray[1] = colorA[1]; // g
    m_trajectoryColorArray[2] = colorA[2]; // b
    m_trajectoryColorArray[3] = 1.; // alpha

    for (unsigned int j=4; j < m_trajectorySize*4; j+=4) 
    {
        std::array<double, 3> color;
        double val = ((double) j) / 4.0 / ((double) m_trajectorySize);
        for (int i = 0; i < 3; ++i)
            color[i] = colorA[i] + val * (colorB[i] - colorA[i]);
        m_trajectoryColorArray[j]   = color[0]; // r
        m_trajectoryColorArray[j+1] = color[1]; // g
        m_trajectoryColorArray[j+2] = color[2]; // b
        m_trajectoryColorArray[j+3] = 1.; // alpha
    }
}

}
