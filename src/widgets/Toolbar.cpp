#include<Windows.h>
#include <QApplication>
#include <QToolTip>
#include "abstractlogger.h"
#include "debugapi.h"
#include <QTime>
#include "Toolbar.h"

void myMessageHandler(QString& msg)
{
    QString formatMsg = QTime::currentTime().toString() + "\t" + msg + "\n";
    OutputDebugStringW(formatMsg.toStdWString().c_str());
}

ToolBarWindow::ToolBarWindow(QWidget* parent)
  : QWidget(parent)
{
    // 设置窗口属性
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    // 启用工具提示
    setAttribute(Qt::WA_AlwaysShowToolTips);
}

void ToolBarWindow::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    //adjustPosition(); // 确保窗口显示时调整位置
}

void ToolBarWindow::updatePosition(const QRect& selection) {
    myMessageHandler(QString("ToolBarWindow::updatePosition %1, %2 \n")
                       .arg(selection.left())
                       .arg(selection.bottom()));
    if (!parentWidget())
        return;

    // 获取父窗口的位置和大小
    QRect parentGeometry = parentWidget()->geometry();

    // 计算工具窗口的尺寸
    QSize toolBarSize = sizeHint();

    // 默认放置在父窗口左下角
    QPoint position(selection.right() - selection.width(),
                    selection.bottom());

    // 先处理基本场景
    for (auto it : m_screenRegions) {
        if (it.contains(QPoint(selection.right(), selection.bottom()))) {
            if (selection.bottom() + toolBarSize.height() > it.bottom()) {
                position.setY(it.bottom() - toolBarSize.height());
                myMessageHandler(
                  QString("ToolBarWindow::m_screenRegions move %1, %2 \n")
                    .arg(position.x())
                    .arg(position.y()));
            }
            break;
        }
    }

    myMessageHandler(QString("ToolBarWindow::updatePosition move %1, %2 \n")
                       .arg(position.x())
                       .arg(position.y()));
    // 移动窗口到计算好的位置
    move(position);
}

void ToolBarWindow::setButtons(const QVector<CaptureToolButton*>& v)
{
    if (v.isEmpty()) {
        return;
    }

    for (CaptureToolButton* b : m_vectorButtons) {
        delete (b);
    }
    m_vectorButtons = v;

    if (m_hLayout)
        delete m_hLayout;
    m_hLayout = nullptr;

    m_hLayout = new QHBoxLayout(this);
    m_hLayout->setContentsMargins(5, 5, 5, 5);
    m_hLayout->setSpacing(5);

    // 添加按钮到布局，并设置事件处理
    for (int i = 0; i < m_vectorButtons.size(); ++i) {
        CaptureToolButton*  btn = m_vectorButtons[i];
        QPushButton* button = new QPushButton(btn->tool()->name(), this);
        if (!button)
            continue;
        m_mapButtons[button] = btn;

        button->setFixedSize(30, 30);    // 设置按钮大小
        button->setToolTip(btn->tool()->description()); // 设置工具提示
        button->setStyleSheet(
          "QPushButton { background-color: #bebebe; border: 1px solid #ccc; "
          "border-radius: 5px; }"
          "QPushButton:hover { background-color: #e0e0ff; }");
        button->setIcon(btn->icon());
        button->setIconSize(size() * 0.6);
        button->setText("");
        // 连接按钮点击事件
        connect(button, &QPushButton::clicked, this, [this, button]() {
            auto btnIter = m_mapButtons.find(button);
            if (btnIter != m_mapButtons.end()) {
                void* _a[] = { nullptr,
                               const_cast<void*>(reinterpret_cast<const void*>(
                                 std::addressof(*btnIter))) };
                QMetaObject::activate(
                  *btnIter, &CaptureToolButton::staticMetaObject, 0, _a);
                myMessageHandler(
                  QString("You clicked: %1").arg(button->text()));            
            }
        });

        m_hLayout->addWidget(button);
    }

    // 设置布局
    setLayout(m_hLayout);
}

void ToolBarWindow::updateScreenRegions(const QRect& rect)
{
    m_screenRegions = { rect };
}

void ToolBarWindow::updateScreenRegions(const QVector<QRect>& rects)
{
    m_screenRegions = rects;
}