
### 

// button 列表的位置更新是在这个里面处理的
void ButtonHandler::updatePosition(const QRect& selection)

void CaptureWidget::initSelection()

// 截图的布局看起来是在这部分处理的
ui_capturelauncher.h

// 插入了所有按钮
m_buttonHandler->setButtons(vectorButtons);

m_buttonHandler = new ButtonHandler(this);
m_buttonHandler->updateScreenRegions(areas);
m_buttonHandler->hide();

###

// 按钮事件
void CaptureWidget::handleToolSignal(CaptureTool::Request r)

CaptureToolButton::getPriorityByButton

// 列表初始化
void ButtonListView::initButtonList()

### 配置路径相关

QString ConfigHandler::configFilePath() const
{
    return m_settings.fileName();
}

###

添加新的 .cpp .h 可能会遇到链接错误，需要关注下是否生成了对应的 moc_*.cpp 文件，没有生成的话，可以修改 CMakeLists.txt 添加对应的文件重新cmake之后，再重新生成解决方案。
error LNK2001: 无法解析的外部符号 "public: virtual struct QMetaObject const * __thiscall ToolBarWindow::metaObject(void)const " (?metaObject@ToolBarWindow@@UBEPBUQMetaObject@@XZ)

