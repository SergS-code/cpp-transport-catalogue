#include "svg.h"

namespace svg {

using namespace std::literals;

void Object::Render(const RenderContext& context) const {
    context.RenderIndent();

    // Делегируем вывод тега своим подклассам
    RenderObject(context);

    context.out << std::endl;
}

// ---------- Circle ------------------

Circle& Circle::SetCenter(Point center)  {
    center_ = center;
    return *this;
}

Circle& Circle::SetRadius(double radius)  {
    radius_ = radius;
    return *this;
}

Circle &Circle::SetFill(std::string fill_)
{
    fill=fill_;
    return *this;
}

void Circle::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "  <circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
    out << "r=\""sv << radius_ << "\" "sv;
    out << "fill=\""sv << fill <<"\""sv;
    out << "/>"sv;
}

// ---------- Polyline ------------------

Polyline &Polyline::AddPoint(Point point)
{
    points.push_back(point);
    return *this;

}

std::string &Polyline::GetFill()
{
    return fill;
}

std::string &Polyline::GetStroke()
{
    return stroke;
}

std::string &Polyline::GetStroke_width()
{
    return stroke_width;
}

std::string &Polyline::GetStroke_linecap()
{
    return stroke_linecap;
}

std::string &Polyline::GetStroke_linejoin()
{
    return stroke_linejoin;
}

void Polyline::RenderObject(const RenderContext &context) const
{
    auto& out = context.out;

    out << "  <polyline points=\""sv;
    for(size_t i=0;i<points.size();++i){
        if(i!=0)
            out<<" "<<points[i].x<<","<<points[i].y;
        else{
            out<<points[i].x<<","<<points[i].y;
        }

    }
    out<<"\"";

    out<<" fill="<<"\""<<fill<<"\""<<" "<<"stroke="<<"\""<<stroke<<"\""<<" "<<"stroke-width="<<"\""<<stroke_width<<"\""
      <<" "<<"stroke-linecap="<<"\""<<stroke_linecap<<"\""<<" "<<"stroke-linejoin="<<"\""<<stroke_linejoin<<"\"";
    out << "/>";
    //  <polyline points="20,40 22.9389,45.9549 29.5106,46.9098 24.7553,51.5451 25.8779,58.0902 20,55 14.1221,58.0902 15.2447,51.5451 10.4894,46.9098 17.0611,45.9549 20,40" />

}

Text &Text::SetPosition(Point pos)
{
    // <text x="35" y="20" dx="0" dy="6" font-size="12" font-family="Verdana" font-weight="bold">Hello C++</text>
    x=pos.x;
    y=pos.y;
    return *this;

}

Text &Text::SetOffset(Point offset)
{
    dx=offset.x;
    dy=offset.y;
    return *this;
}

Text &Text::SetFontSize(uint32_t size)
{
    font=size;
    return *this;
}

Text &Text::SetFontFamily(std::string font_family)
{
    font_family_=font_family;
    return *this;
}

Text &Text::SetFontWeight(std::string font_weight)
{
    font_weight_=font_weight;
    return *this;
}

Text &Text::SetData(std::string data)
{

    size_t s=data.size();
    data_=data;
    for(size_t i =0;i<s;++i){
        if(data[i]=='\"'){
            data.erase(i,1);
            data.insert(i,"&quot;");
            s=data.size();
            i+=6;
        }
        if(data[i]=='\''){
            data.erase(i,1);
            data.insert(i,"&apos;");
            s=data.size();
            i+=6;
        }
        if(data[i]=='<'){
            data.erase(i,1);
            data.insert(i,"&lt;");
            s=data.size();
            i+=4;
        }
        if(data[i]=='>'){
            data.erase(i,1);
            data.insert(i,"&gt;");
            s=data.size();
            i+=4;
        }
        if(data[i]=='&'){
            data.erase(i,1);
            data.insert(i,"&amp;");
            s=data.size();
            i+=5;
        }
    }

    return *this;
}

Text &Text::SetFill(std::string stoke)
{
    fill=stoke;
    return *this;
}

Text &Text::SetStroke(std::string stroke_)
{
    stroke=stroke_;
    return *this;
}

Text &Text::SetStroke_width(std::string stroke_)
{
    stroke_width=stroke_;
    return *this;
}

Text &Text::SetFont_size(std::string stroke_)
{
    font_size=stroke_;
    return *this;
}

void Text::RenderObject(const RenderContext &context) const
{
    auto& out = context.out;
    if(zaliv){
        // out << "<text fill=\""<<fill <<"\""<<" "<<"x=\""<<x<<"\" "<<"y=\""<<y<<"\" "<<"dx=\""<<dx<<"\" "<<"dy=\""<<dy<<"\" "<<"font-size=\""<<font<<"\">"<<data_<<"</text>";
        out << "  <text fill=\""<<fill<<"\""<<" stroke=\""<<stroke<<"\""<< " stroke-width=\""<<stroke_width<<"\""<< " stroke-linecap=\""<<"round"<<"\"";
        out << " stroke-linejoin="<<"\""<<"round"<<"\""<< " x="<<"\""<<x<<"\""<<" y="<<"\""<<y<<"\""<<" dx=" <<"\""<<dx<<"\""<<" dy=" <<"\""<<dy<<"\"";
        out << " font-size="<<"\""<< font<<"\""<<" font-family="<<"\""<<"Verdana";
        if(font_weight_.size()!=0){
            out<<"\""<<" font-weight="<<"\""<<font_weight_;
        }

        out <<"\">"<<data_<<"</text>";
    }else{
        out<<"  <text fill="<<"\""<<fill<<"\"" << " x="<<"\""<<x<<"\""<< " y="<<"\""<< y <<"\""<<" dx="<<"\""<<dx<<"\""
          <<" dy="<<"\"" <<dy<<"\""<< " font-size="<<"\""<< font<<"\""<<" font-family="<<"\""<<"Verdana";
        if(font_weight_.size()!=0){
             out<<"\""<<" font-weight="<<"\""<<font_weight_;
        }
        out <<"\">"<<data_<<"</text>";
    }

    //    if(font_family_.size()==0 && font_weight_.size()==0){
    //        out << "<text x=\""<<x<<"\" "<<"y=\""<<y<<"\" "<<"dx=\""<<dx<<"\" "<<"dy=\""<<dy<<"\" "<<"font-size=\""<<font<<"\">"<<data_<<"</text>";
    //    }
    //    if(font_family_.size()!=0 && font_weight_.size()==0){

    //        out << "<text x=\""<<x<<"\" "<<"y=\""<<y<<"\" "<<"dx=\""<<dx<<"\" "<<"dy=\""<<dy<<"\" "<<"font-size=\""<<font<<"\" "
    //        <<"font-family=\""<<font_family_<<"\">"<<data_<<"</text>";
    //    }

    //    if(font_family_.size()==0 && font_weight_.size()!=0){

    //        out << "<text x=\""<<x<<"\" "<<"y=\""<<y<<"\" "<<"dx=\""<<dx<<"\" "<<"dy=\""<<dy<<"\" "<<"font-size=\""<<font<<"\" "
    //        <<"font-weight=\""<<font_weight_<<"\">"<<data_<<"</text>";
    //    }
    //    if(font_family_.size()!=0 && font_weight_.size()!=0){
    //        out << "<text x=\""<<x<<"\" "<<"y=\""<<y<<"\" "<<"dx=\""<<dx<<"\" "<<"dy=\""<<dy<<"\" "<<"font-size=\""<<font<<"\" "
    //    <<"font-family=\""<<font_family_<<"\" "<<"font-weight=\""<<font_weight_<<"\">"<<data_<<"</text>";
    //    }
    // <text fill="rgba(255,255,255,0.85)" stroke="rgba(255,255,255,0.85)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round" x="99.2283" y="329.5" dx="7" dy="15" font-size="20" font-family="Verdana" font-weight="bold">114</text>
    //<text fill="green" x="99.2283" y="329.5" dx="7" dy="15" font-size="20" font-family="Verdana" font-weight="bold">114</text>
}

void Document::AddPtr(std::unique_ptr<Object> &&obj)
{
    objects_.push_back(std::move(obj));
}

void Document::Render(std::ostream &out) const
{

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv << std::endl;
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv << std::endl;
    for (auto& object : objects_)
    {
        object->Render((RenderContext(out)));
    }
    out << "</svg>"sv;


}

}  // namespace svg
