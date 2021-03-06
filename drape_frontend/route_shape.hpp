#pragma once

#include "drape_frontend/color_constants.hpp"
#include "drape_frontend/map_shape.hpp"
#include "drape_frontend/shape_view_params.hpp"

#include "drape/glstate.hpp"
#include "drape/render_bucket.hpp"
#include "drape/utils/vertex_decl.hpp"
#include "drape/pointers.hpp"

#include "geometry/polyline2d.hpp"

#include "std/vector.hpp"

namespace df
{

double const kArrowSize = 0.001;

struct RouteJoinBounds
{
  double m_start = 0;
  double m_end = 0;
  double m_offset = 0;
};

struct RouteRenderProperty
{
  dp::GLState m_state;
  vector<drape_ptr<dp::RenderBucket>> m_buckets;
  RouteRenderProperty() : m_state(0, dp::GLState::GeometryLayer) {}
};

struct ArrowRenderProperty
{
  vector<RouteJoinBounds> m_joinsBounds;
  vector<double> m_turns;
  double m_start;
  double m_end;
  RouteRenderProperty m_arrow;
};

struct RouteData
{
  m2::PolylineD m_sourcePolyline;
  vector<double> m_sourceTurns;
  df::ColorConstant m_color;
  m2::RectF m_arrowTextureRect;
  double m_length;
  RouteRenderProperty m_route;
  vector<drape_ptr<ArrowRenderProperty>> m_arrows;
};

struct RouteSignData
{
  RouteRenderProperty m_sign;
  bool m_isStart;
  bool m_isValid;
  m2::PointD m_position;
};

class RouteShape
{
public:
  RouteShape(CommonViewParams const & params);
  void CacheRouteSign(ref_ptr<dp::TextureManager> mng, RouteSignData & routeSignData);
  void Draw(ref_ptr<dp::TextureManager> textures, RouteData & routeData);

private:
  using RV = gpu::RouteVertex;
  using TGeometryBuffer = buffer_vector<gpu::RouteVertex, 128>;

  void PrepareGeometry(bool isRoute, vector<m2::PointD> const & path,
                       TGeometryBuffer & geometry, TGeometryBuffer & joinsGeometry,
                       vector<RouteJoinBounds> & joinsBounds, double & outputLength);
  void BatchGeometry(dp::GLState const & state, TGeometryBuffer & geometry,
                     TGeometryBuffer & joinsGeometry, RouteRenderProperty & property);

  CommonViewParams m_params;
};

} // namespace df
