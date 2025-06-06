import type { ReactNode } from 'react';
import clsx from 'clsx';
import Heading from '@theme/Heading';
import Link from '@docusaurus/Link';
import styles from './styles.module.css';

type FeatureItem = {
  title: string;
  // Svg: React.ComponentType<React.ComponentProps<'svg'>>;
  Img?: string;
  description: ReactNode;
  link?: string;
};

const FeatureList: FeatureItem[] = [
  {
    title: 'Powered by InspireFace',
    // Svg: require('@site/static/img/undraw_docusaurus_mountain.svg').default,
    link: 'https://github.com/HyperInspire/InspireFace',
    Img: require('@site/static/img/bob.png').default,
    description: (
      <>
        All features are powered by the high-performance InspireFace C++ SDK.
        Supports CPU, GPU, and NPU.
      </>
    ),
  },
  {
    title: 'Built with Nitro Modules',
    Img: require('@site/static/img/nos.png').default,
    link: 'https://nitro.margelo.com/',
    description: (
      <>
        Built using JSI + C++. Zero bridge overhead, real native performance in
        your React Native app.
      </>
    ),
  },
  {
    title: 'Real-Time Capabilities',
    // Svg: require('@site/static/img/undraw_docusaurus_react.svg').default,
    Img: require('@site/static/img/real-time.png').default,
    description: (
      <>
        Detect, track, and analyze faces in real-time. Perfect for apps needing
        speed, accuracy, and flexibility.
      </>
    ),
  },
];

function Feature({ title, Img, description, link }: FeatureItem) {
  const imageContent = Img && (
    <img src={Img} alt={title} className={styles.featureSvg} />
  );

  return (
    <div className={clsx('col col--4')}>
      <div className="text--center">
        {/* <Svg className={styles.featureSvg} role="img" /> */}
        {link ? <Link to={link}>{imageContent}</Link> : imageContent}
      </div>
      <div className="text--center padding-horiz--md">
        <Heading as="h3">{title}</Heading>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures(): ReactNode {
  return (
    <section className={styles.features}>
      <div className="container padding-top--xl">
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}
