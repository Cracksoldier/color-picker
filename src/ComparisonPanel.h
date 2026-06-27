#pragma once
#include <QWidget>
#include <QColor>

class QLabel;

class ComparisonPanel : public QWidget {
    Q_OBJECT
public:
    explicit ComparisonPanel(QWidget* parent = nullptr);

public slots:
    void setColorA(QColor c);
    void setColorB(QColor c);

private:
    void updateMetrics();
    void updateSwatch(QLabel* swatch, QLabel* hex, QColor c);
    static QString badgeStyle(bool pass);

    QColor  m_colorA;
    QColor  m_colorB;

    QLabel* m_swatchA;
    QLabel* m_hexA;
    QLabel* m_swatchB;
    QLabel* m_hexB;

    QLabel* m_ratioLabel;
    QLabel* m_aaNormalBadge;
    QLabel* m_aaLargeBadge;
    QLabel* m_aaaNormalBadge;
    QLabel* m_aaaLargeBadge;
};
