#include "QGradientSlider.hpp"

#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QLinearGradient>
#include <QStyleOptionSlider>


ColorPicker::QGradientSlider::QGradientSlider(QWidget* parent) :
	QGradientSlider(Qt::Horizontal, parent)
{}

ColorPicker::QGradientSlider::QGradientSlider(Qt::Orientation orientation, QWidget* parent) :
	QSlider(orientation, parent), _privateMember(new Private)
{
	setTickPosition(NoTicks);
}

ColorPicker::QGradientSlider::~QGradientSlider()
{
	delete _privateMember;
}

void ColorPicker::QGradientSlider::mousePressEvent(QMouseEvent* mouseEvent)
{
	if ( mouseEvent->button() == Qt::LeftButton ) {
		mouseEvent->accept();
		setSliderDown(true);
		_privateMember->mouse_event(mouseEvent, this);
		update();
	} else {
		QSlider::mousePressEvent(mouseEvent);
	}
}

void ColorPicker::QGradientSlider::mouseMoveEvent(QMouseEvent* mouseEvent)
{
	if ( mouseEvent->buttons() & Qt::LeftButton ) {
		mouseEvent->accept();
		_privateMember->mouse_event(mouseEvent, this);
		update();
	} else {
		QSlider::mouseMoveEvent(mouseEvent);
	}
}

void ColorPicker::QGradientSlider::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
	if ( mouseEvent->button() == Qt::LeftButton ) {
		mouseEvent->accept();
		setSliderDown(false);
		update();
	} else {
		QSlider::mousePressEvent(mouseEvent);
	}
}

QBrush ColorPicker::QGradientSlider::background() const
{
	return _privateMember->background;
}

void ColorPicker::QGradientSlider::setBackground(const QBrush& background)
{
	_privateMember->background = background;
	update();
	emit backgroundChanged(background);
}

QGradientStops ColorPicker::QGradientSlider::colors() const
{
	return _privateMember->gradient.stops();
}

void ColorPicker::QGradientSlider::setColors(const QGradientStops& colors)
{
	_privateMember->gradient.setStops(colors);
	update();
}

QLinearGradient ColorPicker::QGradientSlider::gradient() const
{
	return _privateMember->gradient;
}

void ColorPicker::QGradientSlider::setGradient(const QLinearGradient& gradient)
{
	_privateMember->gradient = gradient;
	update();
}

void ColorPicker::QGradientSlider::setColors(const QVector<QColor>& colors)
{
	QGradientStops stops;
	stops.reserve(colors.size());

	double color = colors.size() - 1;
	if ( color == 0 ) {
		stops.append(QGradientStop(0, colors.at(0)));
	} else {
		for ( int i = 0; i < colors.size(); ++i ) {
			stops.append(QGradientStop(i / color, colors.at(i)));
		}
	}
	setColors(stops);
}

void ColorPicker::QGradientSlider::setFirstColor(const QColor& color)
{
	QGradientStops stops = _privateMember->gradient.stops();
	if ( stops.isEmpty() ) {
		stops.push_back(QGradientStop(0.0, color));
	} else {
		stops.front().second = color;
	}

	_privateMember->gradient.setStops(stops);
	update();
}

void ColorPicker::QGradientSlider::setLastColor(const QColor& color)
{
	QGradientStops stops = _privateMember->gradient.stops();
	if ( stops.size() < 2 ) {
		stops.push_back(QGradientStop(1.0, color));
	} else {
		stops.back().second = color;
	}

	_privateMember->gradient.setStops(stops);
	update();
}

QColor ColorPicker::QGradientSlider::firstColor() const
{
	QGradientStops gradientStops = colors();
	return gradientStops.empty() ? QColor() : gradientStops.front().second;
}

QColor ColorPicker::QGradientSlider::lastColor() const
{
	QGradientStops gradientStops = colors();
	return gradientStops.empty() ? QColor() : gradientStops.back().second;
}

void ColorPicker::QGradientSlider::paintEvent(QPaintEvent*)
{
	QPainter painter(this);

	QStyleOptionFrame panel;
	panel.initFrom(this);
	panel.lineWidth = 1;
	panel.midLineWidth = 0;
	panel.state |= QStyle::State_Sunken;
	style()->drawPrimitive(QStyle::PE_Frame, &panel, &painter, this);
	QRect r = style()->subElementRect(QStyle::SE_FrameContents, &panel, this);
	painter.setClipRect(r);

	qreal gradient_direction = invertedAppearance() ? -1 : 1;

	if ( orientation() == Qt::Horizontal ) {
		_privateMember->gradient.setFinalStop(gradient_direction, 0);
	} else {
		_privateMember->gradient.setFinalStop(0, -gradient_direction);
	}

	painter.setPen(Qt::NoPen);
	painter.setBrush(_privateMember->background);
	painter.drawRect(1, 1, geometry().width() - 2, geometry().height() - 2);
	painter.setBrush(_privateMember->gradient);
	painter.drawRect(1, 1, geometry().width() - 2, geometry().height() - 2);

	qreal pos = (maximum() != 0) ? static_cast<qreal>(value() - minimum()) / maximum() : 0;
	QColor color;
	auto stops = _privateMember->gradient.stops();
	int i;
	for ( i = 0; i < stops.size(); i++ ) {
		if ( stops[i].first > pos ) {
			break;
		}
	}

	if ( i == 0 ) {
		color = firstColor();
	} else if ( i == stops.size() ) {
		color = lastColor();
	} else {
		auto& a = stops[i - 1];
		auto& b = stops[i];
		auto c = (b.first - a.first);
		qreal q = (c != 0) ? (pos - a.first) / c : 0;
		color = QColor::fromRgbF(b.second.redF() * q + a.second.redF() * (1.0 - q), b.second.greenF() * q + a.second.greenF() * (1.0 - q),
			b.second.blueF() * q + a.second.blueF() * (1.0 - q), b.second.alphaF() * q + a.second.alphaF() * (1.0 - q));
	}

	pos = pos * (geometry().width() - 5);
	if ( color.valueF() > 0.5 || color.alphaF() < 0.5 ) {
		painter.setPen(QPen(Qt::black, 3));
	} else {
		painter.setPen(QPen(Qt::white, 3));
	}

	QPointF p1 = QPointF(2.5, 2.5) + QPointF(pos, 0);
	QPointF p2 = p1 + QPointF(0, geometry().height() - 5);
	painter.drawLine(p1, p2);
}