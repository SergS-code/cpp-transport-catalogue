#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace svg {

struct Point {
    Point() = default;
    Point(double x, double y)
        : x(x)
        , y(y) {
    }
    double x = 0;
    double y = 0;
};

/*
 * Вспомогательная структура, хранящая контекст для вывода SVG-документа с отступами.
 * Хранит ссылку на поток вывода, текущее значение и шаг отступа при выводе элемента
 */
struct RenderContext {
    RenderContext(std::ostream& out)
        : out(out) {
    }

    RenderContext(std::ostream& out, int indent_step, int indent = 0)
        : out(out)
        , indent_step(indent_step)
        , indent(indent) {
    }

    RenderContext Indented() const {
        return {out, indent_step, indent + indent_step};
    }

    void RenderIndent() const {
        for (int i = 0; i < indent; ++i) {
            out.put(' ');
        }
    }

    std::ostream& out;
    int indent_step = 0;
    int indent = 0;
};

/*
 * Абстрактный базовый класс Object служит для унифицированного хранения
 * конкретных тегов SVG-документа
 * Реализует паттерн "Шаблонный метод" для вывода содержимого тега
 */
class Object {
public:
    void Render(const RenderContext& context) const;

    virtual ~Object() = default;

private:
    virtual void RenderObject(const RenderContext& context) const = 0;
};




class ObjectContainer {
public:
    template <typename Obj>
    void Add(Obj obj) {
        AddPtr(std::make_unique<Obj>(std::move(obj)));
    }

    virtual void AddPtr(std::unique_ptr<Object>&& obj)=0;

protected:
    virtual ~ObjectContainer()=default;

};


class Drawable {
public:
    virtual void Draw(ObjectContainer& g) const = 0;
     virtual ~Drawable()=default;
};


/*
 * Класс Circle моделирует элемент <circle> для отображения круга
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/circle
 */
class Circle final : public Object {
public:
    Circle& SetCenter(Point center);
    Circle& SetRadius(double radius);
    Circle& SetFill(std::string fill_);

private:
    void RenderObject(const RenderContext& context) const override;
    std::string fill;
    Point center_;
    double radius_ = 1.0;
};

/*
 * Класс Polyline моделирует элемент <polyline> для отображения ломаных линий
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polyline
 */
class Polyline final : public Object {
public:
    // Добавляет очередную вершину к ломаной линии
    Polyline& AddPoint(Point point);
    std::string& GetFill();
    std::string& GetStroke();
    std::string& GetStroke_width();
    std::string& GetStroke_linecap();
    std::string& GetStroke_linejoin();

private:
    void RenderObject(const RenderContext& context) const override;
    std::vector<Point> points;
    std::string fill="none";
    std::string stroke;
    std::string stroke_width="14";
    std::string stroke_linecap="round";
    std::string stroke_linejoin="round";


};

/*
 * Класс Text моделирует элемент <text> для отображения текста
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text
 */
class Text final : public Object {
public:


    // Задаёт координаты опорной точки (атрибуты x и y)
    Text& SetPosition(Point pos);

    // Задаёт смещение относительно опорной точки (атрибуты dx, dy)
    Text& SetOffset(Point offset);

    // Задаёт размеры шрифта (атрибут font-size)
    Text& SetFontSize(uint32_t size);

    // Задаёт название шрифта (атрибут font-family)
    Text& SetFontFamily(std::string font_family);

    // Задаёт толщину шрифта (атрибут font-weight)
    Text& SetFontWeight(std::string font_weight);

    // Задаёт текстовое содержимое объекта (отображается внутри тега text)
    Text& SetData(std::string data);
    Text& SetFill(std::string stoke);
    Text& SetStroke(std::string stroke_);
    Text& SetStroke_width(std::string stroke_);
    Text& SetFont_size(std::string stroke_);
    bool zaliv;
  private:

    double x=0.0;
    double y =0.0;
    double dx=0.0;
    double dy =0.0;
    uint32_t font=1;
    std::string fill;
    std::string stroke;
    std::string stroke_width;
    std::string font_family_;
    std::string font_weight_;
    std::string font_size;
    std::string data_;

    void RenderObject(const RenderContext& context) const override;
};

class Document:public ObjectContainer {
public:
    /*
     Метод Add добавляет в svg-документ любой объект-наследник svg::Object.
     Пример использования:
     Document doc;
     doc.Add(Circle().SetCenter({20, 30}).SetRadius(15));
    */
    // void Add(???);

    // Добавляет в svg-документ объект-наследник svg::Object
    void AddPtr(std::unique_ptr<Object>&& obj) override ;

    // Выводит в ostream svg-представление документа
    void Render(std::ostream& out) const;



 private:
    std::vector<std::unique_ptr<Object>>objects_;
};

}  // namespace svg
