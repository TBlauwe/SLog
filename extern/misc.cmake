# ------------------------------------------------------------------------------ 
#           File : extern/misc.cmake
#    Description : Download and build all additionnal dependencies using CPM :
#                  * tracy
#                  * fmt
#
#                  Version of each dependency can be set through options using
#                  CPM_<name>_VERSION, e.g CPM_FLECS_VERSION.
#
#                  CPM noteworthy options :
#                  * CPM_SOURCE_CACHE : download location.
#                  * CPM_USE_LOCAL_PACKAGES : search for locally installed dependencies first.
#                  * CPM_LOCAL_PACKAGES_ONLY : error if dependecy not found locally.
# ------------------------------------------------------------------------------ 

# ------------------------------------------------------------------------------
# --- Dependencies
# ------------------------------------------------------------------------------

# FMT is an open-source formatting library providing a fast and safe alternative to C stdio and C++ iostream
