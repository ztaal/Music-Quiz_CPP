#include "QHueSlider.hpp"


ColorPicker::QHueSlider::QHueSlider(QWidget* parent) :
    QGradientSlider(parent), _privateMemeber(new Private(this))
{}

ColorPicker::QHueSlider::QHueSlider(Qt::Orientation orientation, QWidget* parent) :
    QGradientSlider(orientation, parent), _privateMemeber(new Private(this))
{}

ColorPicker::QHueSlider::~QHueSlider()
{
    delete _privateMemeber;
}

qreal ColorPicker::QHueSlider::colorSaturation() const
{
    return _privateMemeber->saturation;
}

void ColorPicker::QHueSlider::setColorSaturation(qreal saturation)
{
    _privateMemeber->saturation = qBound(0.0, saturation, 1.0);
    _privateMemeber->updateGradient();
    emit colorSaturationChanged(saturation);
}

qreal ColorPicker::QHueSlider::colorValue() const
{
    return _privateMemeber->value;
}

void ColorPicker::QHueSlider::setColorValue(qreal value)
{
    _privateMemeber->value = qBound(0.0, value, 1.0);
    _privateMemeber->updateGradient();
    emit colorValueChanged(value);
}

qreal ColorPicker::QHueSlider::colorAlpha() const
{
    return _privateMemeber->alpha;
}

void ColorPicker::QHueSlider::setColorAlpha(qreal alpha)
{
    _privateMemeber->alpha = alpha;
    _privateMemeber->updateGradient();
    emit colorAlphaChanged(alpha);
}

QColor ColorPicker::QHueSlider::color() const
{
    return QColor::fromHsvF(colorHue(), _privateMemeber->saturation, _privateMemeber->value, _privateMemeber->alpha);
}

void ColorPicker::QHueSlider::setColor(const QColor& color)
{
    _privateMemeber->saturation = color.saturationF();
    _privateMemeber->value = color.valueF();
    _privateMemeber->updateGradient();
    setColorHue(color.hueF());
    emit colorValueChanged(_privateMemeber->alpha);
    emit colorSaturationChanged(_privateMemeber->alpha);
}

void ColorPicker::QHueSlider::setFullColor(const QColor& color)
{
    _privateMemeber->alpha = color.alphaF();
    setColor(color);
    emit colorAlphaChanged(_privateMemeber->alpha);
}

qreal ColorPicker::QHueSlider::colorHue() const
{
    if ( maximum() == minimum() ) {
        return 0;
    }

    auto hue = qreal(value() - minimum()) / (maximum() - minimum());
    if ( orientation() == Qt::Vertical ) {
        hue = 1 - hue;
    }
    return hue;
}

void ColorPicker::QHueSlider::setColorHue(qreal colorHue)
{
    if ( orientation() == Qt::Vertical ) {
        colorHue = 1 - colorHue;
    }

    setValue(minimum() + colorHue * (maximum() - minimum()));
    emit colorHueChanged(colorHue);
    emit colorChanged(color());
}