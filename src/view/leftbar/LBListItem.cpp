#include "LBListItem.h"

#include <QDebug>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QTimer>

#include <DThemeManager>

#include "rockrokr_global.h"
#include "widget/RKLineEdit.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

LBListItem::LBListItem(const LBListItem::ItemType &type, const QString &text, QWidget *parent)
    : QFrame(parent),
      m_itemType(type),
      m_text(text)
{
    this->setObjectName("LBListItem");
    DThemeManager::instance()->registerWidget(this);

    m_textEdit = new RKLineEdit;
    m_iconLabel = new QLabel;

    this->setFrameStyle(QFrame::NoFrame);
    this->setContentsMargins(0, 0, 0, 0);
    this->setFixedSize(LEFT_BAR_ITEM_W, LEFT_BAR_ITEM_H);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, //LEFT_BAR_ITEM_CONTENT_MARGIN,
                               0,//LEFT_BAR_ITEM_CONTENT_MARGIN,
                               0, //LEFT_BAR_ITEM_CONTENT_MARGIN,
                               0/*LEFT_BAR_ITEM_CONTENT_MARGIN*/);
    layout->setSpacing(0);

//    m_iconLabel = new QLabel;
    m_iconLabel->setEnabled(false);
    m_iconLabel->setObjectName("TitleIconLabel");
    const QString iconName = enumToStr("ItemType", m_itemType);
    qDebug()<<"--- icon name is "<<iconName;
    m_iconLabel->setProperty("iconName", iconName);

//    m_textEdit = new QLineEdit;
//    m_textEdit->setEnabled(false);
    m_textEdit->setDisabled(true);
    m_textEdit->setReadOnly(true);
    m_textEdit->setObjectName("TitleTextEdit");
    m_textEdit->setFixedHeight(LEFT_BAR_ITEM_H - LEFT_BAR_ITEM_CONTENT_MARGIN *2);
    m_textEdit->setMaxLength(LEFT_BAR_ITEM_W);
    m_textEdit->setProperty("EditValue", m_text);
    QFont f(m_textEdit->font());
    f.setPixelSize(LEFT_BAR_FONT_SIZE_NORMAL);
    m_textEdit->setFont(f);

    QFontMetrics fm(f);

    if (m_itemType == ItemType::TypePlaylist) {
        m_iconLabel->setFixedSize(0, 0);
        m_iconLabel->setVisible(false);

        const int maxw = LEFT_BAR_ITEM_W - LEFT_BAR_ITEM_CONTENT_MARGIN*2;
        m_textEdit->setText(fm.elidedText(text, Qt::TextElideMode::ElideRight, maxw));
        m_textEdit->setFixedWidth(maxw);
    } else {
        m_iconLabel->setFixedSize(LEFT_BAR_ITEM_ICON_PART_W, LEFT_BAR_ITEM_ICON_PART_H);

        const int maxw = LEFT_BAR_ITEM_W - LEFT_BAR_ITEM_ICON_PART_W - LEFT_BAR_ITEM_CONTENT_MARGIN*3;
        m_textEdit->setText(fm.elidedText(text, Qt::TextElideMode::ElideRight, maxw));
//        m_textEdit->setReadOnly(true);
        m_textEdit->setFixedWidth(maxw);
    }

    layout->addWidget(m_iconLabel);
    layout->addWidget(m_textEdit);

    this->setLayout(layout);

    connect(m_textEdit, &QLineEdit::editingFinished,
            this, [&](){
        qDebug()<<"QLineEdit::editingFinished with text "<<m_textEdit->text();

        if (m_textEdit->text().isEmpty()) {
            m_textEdit->setText(m_textEdit->property("EditValue").toString());
        } else {
            m_textEdit->setProperty("EditValue", m_textEdit->text());
        }
        this->setEditMode(false);
        if (m_text != m_textEdit->text()) {
            m_text = m_textEdit->text();

            QFont f(m_textEdit->font());
            f.setPixelSize(LEFT_BAR_FONT_SIZE_NORMAL);
            m_textEdit->setFont(f);

            QFontMetrics fm(f);
            const int maxw = LEFT_BAR_ITEM_W - LEFT_BAR_ITEM_CONTENT_MARGIN*2;
            m_textEdit->setText(fm.elidedText(m_text, Qt::TextElideMode::ElideRight, maxw));

            m_textEdit->setFixedWidth(maxw);
        }
        Q_EMIT this->rename(m_textEdit->text());
    });

    connect(m_textEdit, &QLineEdit::returnPressed,
            this, [&](){
        qDebug()<<"   --- QLineEdit::returnPressed";
        m_textEdit->blockSignals(true);
        this->setFocus();
        m_textEdit->blockSignals(false);
    });
}

LBListItem::~LBListItem()
{

}


QVariant LBListItem::extraData() const
{
    return m_extraData;
}

void LBListItem::setExtraData(const QVariant &extraData)
{
    m_extraData = extraData;
}

void LBListItem::keepHover(bool active)
{
    m_keepHover = active;
    if (!active) {
        this->setHoverState(false);
    }
}

void LBListItem::setHoverState(bool hover)
{
    if (hover) {
        this->setProperty("hoverState", "active");
        m_textEdit->setProperty("hoverState", "active");
    } else {
        this->setProperty("hoverState", "");
        m_textEdit->setProperty("hoverState", "");
    }
    this->style()->unpolish(this);
    this->style()->unpolish(m_textEdit);
    this->style()->polish(this);
    this->style()->polish(m_textEdit);
    this->update();
}

void LBListItem::setEditMode(bool editable)
{
    m_editMode = editable;
    if (editable) {
        m_textEdit->setProperty("editableStatus", "active");
        this->setProperty("editableStatus", "active");
    } else {
        this->setProperty("editableStatus", "");
        m_textEdit->setProperty("editableStatus", "");
        m_textEdit->setDisabled(true);
        m_textEdit->setReadOnly(true);
    }
    this->style()->unpolish(this);
    this->style()->unpolish(m_textEdit);
    this->style()->polish(this);
    this->style()->polish(m_textEdit);
    this->update();
    if (editable) {
        QString text = m_textEdit->property("EditValue").toString();
        qDebug()<<" text is "<<text;
        m_textEdit->setEnabled(true);
        m_textEdit->setReadOnly(false);
        m_textEdit->setText(text);
        m_textEdit->setFocus();
        m_textEdit->setCursorPosition(0);
        m_textEdit->setSelection(0, m_textEdit->text().length());
    }
}


QString LBListItem::enumToStr(const QString &enumName, int enumValue) {
    int index  = metaObject ()->indexOfEnumerator (enumName.toLocal8Bit ());
    QMetaEnum m = metaObject ()->enumerator (index);
    return m.valueToKey (enumValue);
}


void LBListItem::enterEvent(QEvent *event)
{
    QFrame::enterEvent(event);
    if (!m_editMode) {
        this->setHoverState(true);
    }
//    QFrame::enterEvent(event);
}

void LBListItem::leaveEvent(QEvent *event)
{
    QFrame::leaveEvent(event);
    if (!m_editMode && !m_keepHover) {
        this->setHoverState(false);
    }
//    QFrame::leaveEvent(event);
}

void LBListItem::mousePressEvent(QMouseEvent *event)
{
    QFrame::mousePressEvent(event);
    if (event->button() == Qt::MouseButton::LeftButton && !m_editMode) {
        Q_EMIT leftBtnClicked(this);
    }
    if (event->button() == Qt::MouseButton::RightButton) {
        Q_EMIT rightBtnClicked(this, event->pos());
    }
//        QFrame::mousePressEvent(event);
}

int LBListItem::itemType() const
{
    return m_itemType;
}


//void LBListItem::paintEvent(QPaintEvent *event)
//{
//    QStyleOption opt;
//    opt.init(this);
//    QPainter p(this);
//    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//    QWidget::paintEvent(event);
//}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
