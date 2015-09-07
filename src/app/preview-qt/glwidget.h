/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef GLWIDGET_H
#define GLWIDGET_H

// TODO: Simplify to bare minimum
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <QtGlobal> // Q_OS_IOS
#include <QApplication>
#include <QBasicTimer>
#include <QThread>
#include <QImage>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

#include <iostream>
#include <thread>
#include <mutex>

#include <opencv2/core/core.hpp>

namespace gatherer
{
    namespace graphics
    {
        class WarpShader;
        class GLTexture;
    };
};

namespace cv { class VideoCapture; };

#define DO_ORIGINAL 0

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void setVideoDimensions(int width, int height);

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    Q_SLOT void setImage(const cv::Mat &image);

public slots:
    void cleanup();


protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

private:
    
    void initShader();

    std::shared_ptr< gatherer::graphics::WarpShader > m_program;
    std::shared_ptr< gatherer::graphics::GLTexture > m_videoTexture;

    GLuint m_texture = 0;

    GLuint m_videoWidth = 0;
    GLuint m_videoHeight = 0;

    GLuint m_windowWidth = 0;
    GLuint m_windowHeight = 0;

    bool m_core;

    std::mutex m_mutex;
    cv::Mat m_currentFrame; // add a thread safe input queue

    float m_resX = 1.f;
    float m_resY = 1.f;
    
    int m_counter = 0;
};

#endif
