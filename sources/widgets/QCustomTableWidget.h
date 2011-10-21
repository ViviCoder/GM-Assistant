/*************************************************************************
* Copyright © 2011 Vincent Prat & Simon Nicolas
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*************************************************************************/

#ifndef HEADER_QCUSTOMTABLEWIDGET
#define HEADER_QCUSTOMTABLEWIDGET

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMouseEvent>
#include <QMenu>
#include "ChangeSkillDialog.h"
#include "ChangeCharacterDialog.h"
#include "SkillList.h"
#include "CharacterList.h"

class QCustomTableWidget: public QTableWidget
{
    Q_OBJECT

    private:
        QMenu *menuColumn;
        QAction *actionNone, *actionAddColumn, *actionRemoveColumn, *actionAddRow, *actionRemoveRow, *actionEditRow, *actionEditColumn;
        ChangeSkillDialog *pChangeSkillDial;
        ChangeCharacterDialog *pChangeCharacterDial;
        SkillList *pSkills;
        CharacterList *pCharacters;

    protected:
        void mousePressEvent(QMouseEvent *e);
        void keyReleaseEvent(QKeyEvent *e);

    protected slots:
        void onCellChanged(int row, int column);

    public:
        // constructor
        QCustomTableWidget(QWidget *parent=NULL);
        // destructor
        ~QCustomTableWidget();
        // associating a skill list and a character list to the widget
        void setLists(SkillList *skills, CharacterList *chars);
};

#endif
