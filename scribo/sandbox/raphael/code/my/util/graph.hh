template<typename Image, typename Data>
util::graph create_influance_graph(Image ima, Data& area_detected)
{
   image2d<Data> ima_blob = labeling::blobs(ima, c4(), area_detected);
   image2d<Data> ima_influ = transform::influence_zone_geodesic(ima_blob, c4());
   util::graph grph = make::influence_zone_adjacency_graph(ima_influ, c4(), area_detected);
   return grph;
}