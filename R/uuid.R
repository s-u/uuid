UUIDgenerate <- function(use.time = NA) .Call(UUID_gen, use.time)

UUIDvalidate <- function(uuid) {
    stopifnot(is.character(uuid))
    .Call(UUID_validate, uuid)
}
