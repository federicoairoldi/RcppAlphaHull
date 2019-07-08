class Point{
  private:
    double x,y;
  public:
    Point(const double xx, const double yy): x(xx), y(yy) {};

    double getX() const {return x;};
    double getY() const {return y;};
}
