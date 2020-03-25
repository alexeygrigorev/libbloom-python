from setuptools import setup, Distribution

# Tested with wheel v0.29.0
class BinaryDistribution(Distribution):
    """Distribution which always forces a binary package with platform name"""
    def has_ext_modules(foo):
        return True


setup(
    name='pyfvnbloom',
    version='0.0.1',
    description="PyFNVBloom Python Package",
    long_description="PyFNVBloom Python Package",
    install_requires=[
      'numpy',
    ],
    maintainer='Alexey Grigorev',
    maintainer_email='',
    zip_safe=False,
    packages=['pyfvnbloom'],
    package_data={'pyfvnbloom': ['libbloom.so']},
    include_package_data=True,
    license='WTFPL',
    url='bitbucket',
    distclass=BinaryDistribution
)