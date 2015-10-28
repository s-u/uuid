UUIDgenerate <- function(use.time = NA) .Call(UUID_gen, use.time)

##--------------------------------------------------------------------------------
## Vectorized.
## Returns a length(uuid) logical vector.
## If is.na(uuid[i]), then UUIDvalidate(uuid)[i] == NA.
##
## Validation is done via libuuid's uuid_parse, and is thus much faster than using regexs.
##
## Does **not** validate _type_ of UUID (i.e. doesn't distingush between DCE, MD5, MAC access, etc. types).
##--------------------------------------------------------------------------------
UUIDvalidate <- function(uuid) {
    stopifnot(is.character(uuid))
    .Call(UUID_validate, uuid)
}
