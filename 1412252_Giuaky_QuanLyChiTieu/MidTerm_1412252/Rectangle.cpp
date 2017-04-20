#include "stdafx.h"
#include "Rectangle.h"


CRectangle::CRectangle()
{
	m_left = m_top = m_right = m_bottom = 0;
	m_color_R = m_color_G = m_color_B = 1;
	
}


CRectangle::~CRectangle()
{
}

int CRectangle::get_left(){
	return m_left;
}
int CRectangle::set_left(int value) {
	m_left = value;
	return 0;
}
int CRectangle::get_top() {
	return m_top;
}
int CRectangle::set_top(int value) {
	m_top = value;
	return 0;
}
int CRectangle::get_right() {
	return m_right;
}
int CRectangle::set_right(int value) {
	m_right = value;
	return 0;
}
int CRectangle::get_bottom() {
	return m_bottom;
}
int CRectangle::set_bottom(int value) {
	m_bottom = value;
	return 0;
}

COLORREF CRectangle::get_colorRGB()
{
	return m_colorRGB;
}

void CRectangle::set_colorRGB(COLORREF color)
{
	this->m_colorRGB = color;
}

//int CRectangle::get_colorR() {
//	return m_color_R;
//}
//int CRectangle::set_colorR(int value) {
//	m_color_R = value;
//	return 0;
//}
//int CRectangle::get_colorG() {
//	return m_color_G;
//}
//int CRectangle::set_colorG(int value) {
//	m_color_G = value;
//	return 0;
//}
//int CRectangle::get_colorB() {
//	return m_color_B;
//}
//int CRectangle::set_colorB(int value) {
//	m_color_B = value;
//	return 0;
//}
