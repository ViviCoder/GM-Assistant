#ifndef HEADER_QCUSTOMTABLEWIDGET
#define HEADER_QCUSTOMTABLEWIDGET

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMouseEvent>
#include <QMenu>
#include "ChangeHeaderDialog.h"
#include "SkillList.h"
#include "CharacterList.h"

class QCustomTableWidget: public QTableWidget
{
    Q_OBJECT

    private:
        QMenu *menuColumn;
        QAction *actionNone, *actionAddColumn, *actionRemoveColumn, *actionAddRow, *actionRemoveRow, *actionEditRow, *actionEditColumn;
        ChangeHeaderDialog *pChangeHeaderDial;
        SkillList *pSkills;
        CharacterList *pCharacters;

    protected:
        void mousePressEvent(QMouseEvent *e);
        void keyReleaseEvent(QKeyEvent *e);

    public:
        // constructor
        QCustomTableWidget(QWidget *parent=NULL);
        // destructor
        ~QCustomTableWidget();
        // associating a skill list and a character list to the widget
        void setLists(SkillList *skills, CharacterList *chars);
};

#endif
