\name{UUID}
\alias{UUID}
\alias{uuid}
\alias{as.UUID}
\alias{is.UUID}
\docType{data}
\title{
UUID Data Type
}
\description{
  S3 class \code{"UUID"} represents vector of UUIDs in native form
  (128-bit). They are typically obtained by calling
  \code{\link{UUIDgenerate}}, \code{\link{UUIDparse}} or \code{as.UUID}.

  Methods exist for common operations such as \code{\link{as.character}},
  \code{\link{print}}, \code{\link{c}}, subsetting and comparison
  operators. Note that arithmetic and other operations are not allowed.

  UUIDs have three possible representations: as character vectors (in
  the hyphenated 8-4-4-4-12 hexadecimal form), the \code{UUID} class
  described here and raw vectors. In the latter case the raw vector must
  be of length 16 or it must be a matrix with 16 rows. Since matrices in
  R are stored in colum-major format, UUID must be contiguous and thus
  form the \emph{columns} of the raw matrix, which may be slightly
  counter-intuitive, but is far moreefficient.

  \code{as.character} method exist for UUID objects and converts it to a
  character vector of lower-case UUID string representation.

  \code{as.raw} method converts UUIDs to raw vectors or matrices as
  describe above. Similarly, a \code{as.UUID} method for raw vectors
  performs the inverse transformation.
}
\usage{
as.UUID(x, ...)
is.UUID(x)
}
\arguments{
  \item{x}{object to coerce / check}
  \item{...}{unused}
}
\value{
  \code{as.UUID} returns an object of the class \code{"UUID"}
  representing a vector of UUIDs. Any elements that are not
  valid UUIDs will yield \code{NA} values.

  \code{is.UUID} returns \code{TRUE} if the object is of the class
  \code{"UUID"} and \code{FALSE} otherwise.
}
\details{
  Internally, the underlying object uses complex numbers to store 128-bit
  values with each UUID represented as one complex number. There may be
  cases where some operations strip the class attribute which will lead
  to complex values being visible, but their behavior should be regarded
  as undefined.

  \code{NA} values in the UUID class are internally stored as a special
  value a2070000-0000-f07f-a207-00000000f07f which is not a valid UUID
  (since the version of that UUID is 15 which does not exist). This is
  an R extension and will be automatically converted to \code{NA} where
  possible, but the raw format does not support \code{NA}s so it will be
  visible there. Coercions to/from string and UUIDs handle \code{NA}s
  correctly and thus this internal representation should not be relied
  upon by any code and may change in the future.
}
\note{
  Comparisons are much faster between UUID vectors than between UUID
  vectors and other types, because in the latter case all values are
  coerced to strings before comparison which is very expensive.

  However, \code{x == y} does not necessarily yield the same result as
  \code{as.UUID(x) == as.UUID(y)}. For example, for \code{x} a valid
  UUID object of length one and \code{y = "foo"} the former will be
  \code{FALSE} while the latter will be \code{NA} due to coercion not
  yielding a valid UUID value represented by \code{NA}.
}
\author{
  Simon Urbanek
}
\examples{
(u <- as.UUID("837bc850-07d9-42f9-9afb-716409bf87b7"))
(uv <- c(u, NA, UUIDgenerate(n=3, output="uuid")))
as.character(u)
uv == u
is.na(uv)
identical(as.UUID(as.character(uv)), uv)
as.raw(u)

## all forms are can be coerced losslessly
identical(as.UUID(as.raw(uv)), uv)
identical(as.UUID(as.character(as.UUID(as.raw(uv)))), uv)
}
\keyword{manip}
