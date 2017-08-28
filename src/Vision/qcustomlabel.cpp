/*
 * This file is part of the VSS-Vision project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include "qcustomlabel.h"

QCustomLabel::QCustomLabel(QLabel *parent) : QLabel(parent)
{
    x = y = delta = 0;
}

void QCustomLabel::mouseMoveEvent(QMouseEvent *ev){
    this->x = ev->x();
    this->y = ev->y();
    Q_EMIT Mouse_Pos();
}

void QCustomLabel::mousePressEvent(QMouseEvent *ev){
    if(ev->buttons() == Qt::RightButton){
        last_right_click_x = ev->x();
        last_right_click_y = ev->x();
        Q_EMIT Mouse_Right_Pressed();
    }else
    if(ev->buttons() == Qt::LeftButton){
        last_left_click_x = ev->x();
        last_left_click_y = ev->x();
        Q_EMIT Mouse_Left_Pressed();
    }
}

void QCustomLabel::mouseReleaseEvent(QMouseEvent *ev){
    Q_EMIT Mouse_Release();
}

void QCustomLabel::leaveEvent(QEvent *ev){
    Q_EMIT Mouse_Left();
}

//! Addendum
//! --------
//! 
void QCustomLabel::wheelEvent(QWheelEvent *ev){
    volatil = ev->delta()/120;
    //! > Get the variation of the mouse scroll and set a gain to the scroll velocity.
    if(delta + volatil >= 0 && delta + volatil <= 75){
        delta += volatil;
    }
    Q_EMIT Mouse_Scroll();
}

void QCustomLabel::keyPressEvent(QKeyEvent *ev){
    stringstream ss;
    ss << ev->key();
    qDebug() << ss.str().c_str();
    Q_EMIT Key_Pressed();
}

