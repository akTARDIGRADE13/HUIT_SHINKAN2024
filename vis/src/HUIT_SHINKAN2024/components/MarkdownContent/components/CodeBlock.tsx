import { type FC, type ClassAttributes, type HTMLAttributes } from 'react';
import type React from 'react';
import { type ExtraProps } from 'react-markdown';
import SyntaxHighlighter from 'react-syntax-highlighter/dist/esm/default-highlight';
import { tomorrowNight } from 'react-syntax-highlighter/dist/esm/styles/hljs';

interface ChildProps {
  children?: React.ReactNode;
  className?: string;
}

interface CodeBlockProps
  extends ClassAttributes<HTMLPreElement>,
    HTMLAttributes<HTMLPreElement>,
    ExtraProps {
  children?: React.ReactNode;
}

const CodeBlock: FC<CodeBlockProps> = ({ children, ...props }) => {
  if (children === null || typeof children !== 'object') {
    return <code {...props}>{children}</code>;
  }
  const childType = 'type' in children ? children.type : '';
  if (childType !== 'code') {
    return <code {...props}>{children}</code>;
  }

  const childProps: ChildProps =
    'props' in children ? (children.props as ChildProps) : {};
  const { className, children: code } = childProps;
  const language = className?.replace('language-', '');

  return (
    <SyntaxHighlighter language={language} style={tomorrowNight}>
      {String(code).replace(/\n$/, '')}
    </SyntaxHighlighter>
  );
};

export default CodeBlock;
