\name{UUIDgenerate}
\alias{UUIDgenerate}
\title{
  Generate UUID
}
\description{
  \code{UUIDgenerate} generates a new Universally Unique Identifier. It
  can be either time-based or random.
}
\usage{
UUIDgenerate(use.time = NA)
}
\arguments{
  \item{use.time}{logical, if \code{TRUE} then time-based UUID is
    generated, if \code{FALSE} then a radom UUID is generated, if
    \code{NA} then random one is generated is asufficiently reliable
    source of random numbers can be found, otehrwise a time-based UUID
    is generated.}
}
%\details{
%}
\value{
  String, UUID in lowercase form.
}
%\references{
%}
\author{
  Simon Urbanek, based on libuuid by Theodore Ts'o.
}
%\note{
%}
\examples{
UUIDgenerate()
UUIDgenerate(TRUE)
UUIDgenerate(FALSE)
}
\keyword{manip}
