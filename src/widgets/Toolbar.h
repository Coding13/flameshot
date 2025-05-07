#ifndef TOOLBARWINDOW_H
#define TOOLBARWINDOW_H

#include "capturetoolbutton.h"
#include "src/tools/capturetool.h"

#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QWidget>

class CaptureToolButton;

class ToolBarWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBarWindow(QWidget* parent = nullptr);
    void setButtons(const QVector<CaptureToolButton*>& v);
    void updatePosition(const QRect& selection);
    void updateScreenRegions(const QRect& rect);
    void updateScreenRegions(const QVector<QRect>& rects);

protected:
    void showEvent(QShowEvent* event) override;

private slots:

private:

    QVector<CaptureToolButton*> m_vectorButtons;
    QMap<QPushButton*, CaptureToolButton*> m_mapButtons;
    QHBoxLayout* m_hLayout = nullptr;
    QVector<QRect> m_screenRegions;
};

#endif // TOOLBARWINDOW_H